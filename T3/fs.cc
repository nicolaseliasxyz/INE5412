#include "fs.h"

int INE5412_FS::fs_format() {
    // Verificar se o sistema de arquivos já está montado
    if (is_mounted) {
        return 0;
    }

    union fs_block block;

    // Ler o superbloco do disco para obter informações sobre o sistema de arquivos
    disk->read(0, block.data);

    // Definições do superbloco
    block.super.magic = FS_MAGIC;
    block.super.nblocks = disk->size();
    block.super.ninodeblocks = (block.super.nblocks + 9) / 10;
    block.super.ninodes = block.super.ninodeblocks * INODES_PER_BLOCK;

    // Reinicializar o bitmap de inodes e blocos
    inode_bitmap = fs_bitmap(block.super.ninodes);
    block_bitmap = fs_bitmap(block.super.nblocks);

    // Zerar todos os blocos de inodes
    union fs_block zero_block;
    memset(&zero_block, 0, sizeof(zero_block));
    for (int i = 0; i < block.super.ninodeblocks; i++) {
        disk->write(1 + i, zero_block.data);
    }

    // Assegurar que as definições do superbloco sejam mantidas
    disk->write(0, block.data);

    return 1; // Sucesso
}


void INE5412_FS::fs_debug()
{
	union fs_block block;

	disk->read(0, block.data);

	cout << "superblock:\n";
	cout << "    " << (block.super.magic == FS_MAGIC ? "magic number is valid\n" : "magic number is invalid!\n");
 	cout << "    " << block.super.nblocks << " blocks\n";
	cout << "    " << block.super.ninodeblocks << " inode blocks\n";
	cout << "    " << block.super.ninodes << " inodes\n";

    for (int i = 0; i < block.super.ninodeblocks; i++) {
        // Lendo o bloco de inode
        disk->read(1 + i, block.data);

        for (int j = 0; j < INODES_PER_BLOCK; j++) {
            fs_inode inode = block.inode[j];

            // Verificando se o inode é válido
            if (inode.isvalid) {
                cout << "inode " << i * INODES_PER_BLOCK + j + 1 << ":\n";
                cout << "    Size: " << inode.size << " bytes\n";
                cout << "    Direct blocks:";

                // Imprimindo blocos diretos
                for (int k = 0; k < POINTERS_PER_INODE; k++) {
                    if (inode.direct[k]) {
                        cout << " " << inode.direct[k];
                    }
                }

                cout << "\n";

                // Verificar e imprimir bloco indireto, se houver
                if (inode.indirect) {
                    cout << "    Indirect block: " << inode.indirect << "\n";
                    union fs_block indirect_block;
                    disk->read(inode.indirect, indirect_block.data);
                    
                    cout << "    Indirect data blocks:";
                    for (int k = 0; k < POINTERS_PER_BLOCK; k++) {
                        if (indirect_block.pointers[k]) {
                            cout << " " << indirect_block.pointers[k];
                        }
                    }
                    cout << "\n";
                }
            }
        }
    }
}

int INE5412_FS::fs_mount() {
    union fs_block block;

    // Ler o superbloco do disco
    disk->read(0, block.data);

    // Verificar se o sistema de arquivos já está montado
    if (is_mounted) {
        return 0; // Falha se já estiver montado
    }

    // Verificar o número mágico para garantir que é um sistema de arquivos válido
    if (block.super.magic != FS_MAGIC) {
        return 0; // Falha se o número mágico não for o esperado
    }

    // Reinicializar os bitmaps de inodes e blocos
    inode_bitmap = fs_bitmap(block.super.ninodes);
    block_bitmap = fs_bitmap(block.super.nblocks);

    // Atualizar inode_bitmap com base no estado de cada inode
    for (int i = 0; i < block.super.ninodeblocks; i++) {
        disk->read(1 + i, block.data);
        for (int j = 0; j < INODES_PER_BLOCK; j++) {
            if (block.inode[j].isvalid) {
                int inumber = i * INODES_PER_BLOCK + j;
                inode_bitmap.set_used(inumber);

                // Atualizar block_bitmap para blocos diretos
                for (int k = 0; k < POINTERS_PER_INODE; k++) {
                    if (block.inode[j].direct[k] != 0) {
                        block_bitmap.set_used(block.inode[j].direct[k]);
                    }
                }

                // Atualizar block_bitmap para bloco indireto, se houver
                if (block.inode[j].indirect != 0) {
                    block_bitmap.set_used(block.inode[j].indirect);
                    union fs_block indirect_block;
                    disk->read(block.inode[j].indirect, indirect_block.data);

                    // Atualizar para cada bloco de dados apontado pelo bloco indireto
                    for (int k = 0; k < POINTERS_PER_BLOCK; k++) {
                        if (indirect_block.pointers[k] != 0) {
                            block_bitmap.set_used(indirect_block.pointers[k]);
                        }
                    }
                }
            }
        }
    }

    // Marcar o sistema de arquivos como montado
    is_mounted = true;

    return 1; // Sucesso
}

int INE5412_FS::fs_create() {
    // Verificar se o sistema de arquivos está montado
    if (!is_mounted) {
        return 0; // Sistema de arquivos não está montado
    }

    union fs_block block;
    // Ler o superbloco para obter informações sobre o sistema de arquivos
    disk->read(0, block.data);

    // Procurar por um inode livre
    for (int inumber = 0; inumber < block.super.ninodes; inumber++) {
        if (inode_bitmap.is_free(inumber)) {
            // Marcar o inode como utilizado no bitmap
            inode_bitmap.set_used(inumber);

            // Calcular o número do bloco e a posição do inode dentro desse bloco
            int blocknum = inumber / INODES_PER_BLOCK;
            int offset = inumber % INODES_PER_BLOCK;
            int inode_blocknum = blocknum + 1; // O superbloco está no bloco 0

            // Ler o bloco que contém o inode do disco
            disk->read(inode_blocknum, block.data);

            // Inicializar o inode
            block.inode[offset].isvalid = 1;
            block.inode[offset].size = 0;
            memset(block.inode[offset].direct, 0, sizeof(block.inode[offset].direct));
            block.inode[offset].indirect = 0;

            // Escrever o bloco modificado de volta no disco
            disk->write(inode_blocknum, block.data);

            // Retornar o número do inode (ajustado para base 1)
            return inumber + 1;
        }
    }

    // Retornar -1 se nenhum inode livre for encontrado
    return -1;
}

int INE5412_FS::fs_delete(int inumber) {
    if (!is_mounted) {
        return 0; // Sistema de arquivos não está montado
    }
    union fs_block block;

    // Ler o superbloco do disco para obter informações sobre o sistema de arquivos
    disk->read(0, block.data);

    // Verificar se o número do inode é válido
    if (inumber <= 0 || inumber > block.super.ninodes) {
        return 0; // Número de inode inválido
    }

    // Calcular o número do bloco e a posição do inode dentro desse bloco
    int blocknum = (inumber - 1) / INODES_PER_BLOCK;
    int offset = (inumber - 1) % INODES_PER_BLOCK;
    int inode_blocknum = blocknum + 1;

    // Ler o bloco do disco que contém o inode
    disk->read(inode_blocknum, block.data);

    // Verificar se o inode está em uso
    if (!block.inode[offset].isvalid) {
        return 0; // Inode não está em uso
    }

    // Liberar blocos diretos e atualizar o bitmap de blocos livres
    for (int i = 0; i < POINTERS_PER_INODE; i++) {
        if (block.inode[offset].direct[i] != 0) {
            block_bitmap.set_free(block.inode[offset].direct[i]);
            block.inode[offset].direct[i] = 0;
        }
    }

    // Verificar e liberar bloco indireto
    if (block.inode[offset].indirect != 0) {
        union fs_block indirect_block;
        disk->read(block.inode[offset].indirect, indirect_block.data);

        for (int i = 0; i < POINTERS_PER_BLOCK; i++) {
            if (indirect_block.pointers[i] != 0) {
                block_bitmap.set_free(indirect_block.pointers[i]);
                indirect_block.pointers[i] = 0;
            }
        }

        // Escrever as alterações do bloco de ponteiros indiretos de volta no disco
        disk->write(block.inode[offset].indirect, indirect_block.data);

        // Libera o próprio bloco de ponteiros indiretos
        block_bitmap.set_free(block.inode[offset].indirect);
        block.inode[offset].indirect = 0;
    }

    // Marcar o inode como livre
    block.inode[offset].isvalid = 0;
    inode_bitmap.set_free(inumber - 1);

    // Escrever as alterações de volta no disco
    disk->write(inode_blocknum, block.data);

    return 1; // Sucesso
}


int INE5412_FS::fs_getsize(int inumber) {
    if (!is_mounted) {
        return -1; // Falha se o sistema de arquivos não estiver montado
    }

    union fs_block block;
    disk->read(0, block.data);

    if (inumber <= 0 || inumber > block.super.ninodes) {
        return -1; // Número de inode inválido
    }

    int blocknum = (inumber - 1) / INODES_PER_BLOCK;
    int offset = (inumber - 1) % INODES_PER_BLOCK;
    int inode_blocknum = blocknum + 1; // Ajuste para os blocos de inode começarem após o superbloco

    disk->read(inode_blocknum, block.data);

    if (!block.inode[offset].isvalid) {
        return -1; // Inode não está em uso
    }

    return block.inode[offset].size;
}


int INE5412_FS::fs_read(int inumber, char *data, int length, int offset) {
    if (!is_mounted) {
        return -1; // Sistema de arquivos não está montado
    }
    union fs_block block;

    // Ler o superbloco do disco
    disk->read(0, block.data);

    // Verificar se o número do inode é válido
    if (inumber <= 0 || inumber > block.super.ninodes) {
        return -1; // Número de inode inválido
    }

    // Calcular o número do bloco e a posição do inode dentro desse bloco
    int blocknum = (inumber - 1) / INODES_PER_BLOCK;
    int inode_offset = (inumber - 1) % INODES_PER_BLOCK;
    int inode_blocknum = blocknum + 1;

    // Ler o bloco do disco que contém o inode
    disk->read(inode_blocknum, block.data);

    fs_inode inode = block.inode[inode_offset];
    if (!inode.isvalid || offset >= inode.size) {
        return -1; // Inode não está em uso ou offset inválido
    }

    int bytes_to_read = std::min(length, inode.size - offset);
    int bytes_read = 0;

    // Lógica para ler dados dos blocos diretos e indiretos
    while (bytes_to_read > 0) {
        int block_index = offset / Disk::DISK_BLOCK_SIZE;
        int block_offset = offset % Disk::DISK_BLOCK_SIZE;

        int data_block_num;
        if (block_index < POINTERS_PER_INODE) {
            // Ler de um bloco direto
            data_block_num = inode.direct[block_index];
        } else {
            // Ler de um bloco indireto
            union fs_block pointer_block;
            disk->read(inode.indirect, pointer_block.data);
            data_block_num = pointer_block.pointers[block_index - POINTERS_PER_INODE];
        }

        union fs_block data_block;
        disk->read(data_block_num, data_block.data);
        
        int bytes_in_block = std::min(Disk::DISK_BLOCK_SIZE - block_offset, bytes_to_read);
        memcpy(data + bytes_read, data_block.data + block_offset, bytes_in_block);

        bytes_read += bytes_in_block;
        bytes_to_read -= bytes_in_block;
        offset += bytes_in_block;
    }

    return bytes_read; // Retorna o número total de bytes lidos
}

int INE5412_FS::fs_write(int inumber, const char *data, int length, int offset) {
    if (!is_mounted) {
        return -1; // Sistema de arquivos não está montado
    }
    union fs_block block;

    // Ler o superbloco do disco
    disk->read(0, block.data);

    // Verificar se o número do inode é válido
    if (inumber <= 0 || inumber > block.super.ninodes) {
        return -1; // Número de inode inválido
    }

    int blocknum = (inumber - 1) / INODES_PER_BLOCK;
    int inode_offset = (inumber - 1) % INODES_PER_BLOCK;
    int inode_blocknum = blocknum + 1;

    // Ler o bloco do disco que contém o inode
    disk->read(inode_blocknum, block.data);

    fs_inode *inode = &block.inode[inode_offset];
    if (!inode->isvalid) {
        return -1; // Inode não está em uso
    }

    int bytes_written = 0;

    while (bytes_written < length) {
        int block_index = (offset + bytes_written) / Disk::DISK_BLOCK_SIZE;
        int block_offset = (offset + bytes_written) % Disk::DISK_BLOCK_SIZE;

        int data_block_num;

        if (block_index < POINTERS_PER_INODE) {
            if (inode->direct[block_index] == 0) {
                inode->direct[block_index] = allocate_block();
                if (inode->direct[block_index] == -1) {
                    break; // Não há mais blocos livres
                }
                block_bitmap.set_used(inode->direct[block_index]); // Atualizar block_bitmap
            }
            data_block_num = inode->direct[block_index];
        } else {
            if (inode->indirect == 0) {
                inode->indirect = allocate_block();
                if (inode->indirect == -1) {
                    break; // Não há mais blocos livres
                }
                union fs_block pointer_block;
                block_bitmap.set_used(inode->indirect); // Atualizar block_bitmap para o bloco de ponteiros indiretos
                memset(&pointer_block, 0, sizeof(pointer_block)); // Inicializar o bloco de ponteiros indiretos
                disk->write(inode->indirect, pointer_block.data);
            }

            union fs_block pointer_block;
            disk->read(inode->indirect, pointer_block.data);

            int indirect_index = block_index - POINTERS_PER_INODE;
            if (pointer_block.pointers[indirect_index] == 0) {
                pointer_block.pointers[indirect_index] = allocate_block();
                if (pointer_block.pointers[indirect_index] == -1) {
                    break; // Não há mais blocos livres
                }
                block_bitmap.set_used(pointer_block.pointers[indirect_index]); // Atualizar block_bitmap
                disk->write(inode->indirect, pointer_block.data);
            }
            data_block_num = pointer_block.pointers[indirect_index];
        }

        union fs_block data_block;
        disk->read(data_block_num, data_block.data);

        int bytes_in_block = std::min(length - bytes_written, Disk::DISK_BLOCK_SIZE - block_offset);
        memcpy(data_block.data + block_offset, data + bytes_written, bytes_in_block);

        disk->write(data_block_num, data_block.data);

        bytes_written += bytes_in_block;
    }

    // Atualizar o tamanho do inode, se necessário
    if (offset + bytes_written > inode->size) {
        inode->size = offset + bytes_written;
        disk->write(inode_blocknum, block.data); // Escrever as alterações do inode de volta no disco
    }

    return bytes_written; // Retorna o número total de bytes escritos
}


int INE5412_FS::allocate_block() {
    union fs_block block;
    disk->read(0, block.data);

    // Verificar se há blocos livres disponíveis
    for (int i = 1; i < block.super.nblocks; i++) {
        if (block_bitmap.is_free(i)) {
            block_bitmap.set_used(i);
            return i;
        }
    }
    return -1; // Nenhum bloco livre encontrado
}
