#include <stdio.h>  
#include <stdlib.h>
#include <string.h>
#include <windows.h>

typedef struct {
    char usuario[50];
    char senha[50]; 
} Usuario;

void criptografar(char *texto, char chave) {
    int i;
    for (i = 0; texto[i] != '\0'; i++) {
        texto[i] ^= chave; 
    }
}

int login(FILE *arquivo) {
    char usuario[50], senha[50];
    Usuario usuarioArquivo;
    
    printf("Digite o nome do usuario: ");
    scanf("%s", usuario);
    printf("Digite a senha: ");
    scanf("%s", senha);

    // Buscar o usuário no arquivo
    while (fscanf(arquivo, "%s %s", usuarioArquivo.usuario, usuarioArquivo.senha) != EOF) {
        
        // Criptografar a senha do arquivo e a senha informada para comparação
        
        criptografar(senha, 'C');
        
        if (strcmp(usuarioArquivo.usuario, usuario) == 0 && strcmp(usuarioArquivo.senha, senha) == 0) {
            printf("Login bem-sucedido!\n");
            return 1;  // Sucesso no login
        }
    }

    printf("Credenciais incorretas.\n");
    return 0;  // Falha no login
}
void criarUsuarioAdminSeNecessario(FILE *arquivo) {
    fseek(arquivo, 0, SEEK_END);
    long tamanho = ftell(arquivo);
    if (tamanho == 0) {  // Verifica se o arquivo está vazio
        Usuario admin;
        strcpy(admin.usuario, "admin");
        strcpy(admin.senha, "admin123");
        criptografar(admin.senha, 'C');
        fprintf(arquivo, "%s %s\n", admin.usuario, admin.senha);
        printf("Usuário administrador padrão criado:\n user: admin \n senha:admin123\n");
    }
    rewind(arquivo);  // Retorna o ponteiro para o início do arquivo
}

void incluirUsuario(FILE *arquivo) {
    Usuario novoUsuario;
    printf("Digite o nome do usuario: ");
    scanf("%s", novoUsuario.usuario);
    printf("Digite a senha: ");
    scanf("%s", novoUsuario.senha);

    criptografar(novoUsuario.senha, 'C'); 

    
    fprintf(arquivo, "%s %s\n", novoUsuario.usuario, novoUsuario.senha);
}
void listarUsuarios(FILE *arquivo) {
    Usuario usuario;
    while (fscanf(arquivo, "%s %s", usuario.usuario, usuario.senha) != EOF) {
        
        criptografar(usuario.senha, 'C');
        printf("Usuario: %s, Senha: %s\n", usuario.usuario, usuario.senha);
    }
}

void alterarUsuario(FILE *arquivo) {
    char usuarioAlterado[50];
    Usuario usuario;
    FILE *tempArquivo;
    int encontrado = 0;

    tempArquivo = fopen("temp.txt", "w+");
    if (tempArquivo == NULL) {
        printf("Erro ao criar arquivo temporário.\n");
        return;
    }

    printf("Digite o nome do usuario para alterar a senha: ");
    scanf("%s", usuarioAlterado);

    rewind(arquivo);
    while (fscanf(arquivo, "%s %s", usuario.usuario, usuario.senha) != EOF) {
        if (strcmp(usuario.usuario, usuarioAlterado) == 0) {
            encontrado = 1;
            printf("Digite a nova senha para o usuario %s: ", usuarioAlterado);
            scanf("%s", usuario.senha);
            criptografar(usuario.senha, 'C');  // Criptografa a nova senha
        }
        fprintf(tempArquivo, "%s %s\n", usuario.usuario, usuario.senha);  // Escreve no arquivo temporário
    }

    fclose(tempArquivo);
    fclose(arquivo);

    if (encontrado) {
        remove("usuarios.txt");  // Remove o arquivo original
        rename("temp.txt", "usuarios.txt");  // Renomeia o arquivo temporário para o nome original
        printf("Senha do usuário alterada com sucesso!\n");
    } else {
        printf("Usuario não encontrado.\n");
    }

    arquivo = fopen("usuarios.txt", "a+");  // Reabre o arquivo original
}

void excluirUsuario(FILE *arquivo) {
    char usuarioExcluido[50];
    Usuario usuario;
    FILE *tempArquivo;
    int encontrado = 0;

    tempArquivo = fopen("temp.txt", "w+");
    if (tempArquivo == NULL) {
        printf("Erro ao criar arquivo temporário.\n");
        return;
    }

    printf("Digite o nome do usuario para excluir: ");
    scanf("%s", usuarioExcluido);

    rewind(arquivo);
    while (fscanf(arquivo, "%s %s", usuario.usuario, usuario.senha) != EOF) {
        if (strcmp(usuario.usuario, usuarioExcluido) != 0) {
            fprintf(tempArquivo, "%s %s\n", usuario.usuario, usuario.senha);  // Não escreve o usuário excluído
        } else {
            encontrado = 1;  // Marca que encontrou o usuário para excluir
        }
    }

    fclose(tempArquivo);
    fclose(arquivo);

    if (encontrado) {
        remove("usuarios.txt");  
        rename("temp.txt", "usuarios.txt"); 
        printf("Usuário excluído com sucesso!\n");
    } else {
        printf("Usuário não encontrado.\n");
    }

    arquivo = fopen("usuarios.txt", "a+");  
}

int main() {
    
    FILE *arquivo;
    arquivo = fopen("usuarios.txt", "a+");
    
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo.\n");
    }

    criarUsuarioAdminSeNecessario(arquivo);
    

    //Login de usuário
    if (!login(arquivo)) {
        fclose(arquivo);
        return 0;  // Se o login falhar, encerra o programa
    }

    int opcao;
    do {
    
        printf("\n1 - INCLUIR USUARIOS\n2 - LISTAR USUARIOS\n3 - ALTERAR USUARIO\n4 - EXCLUIR USUARIO\n5 - SAIR\n");
        scanf("%d", &opcao);
        
        switch (opcao) {
            case 1:
                incluirUsuario(arquivo);
                break;
            case 2:
                listarUsuarios(arquivo);
                break;
            case 3:
                alterarUsuario(arquivo);
                break;
            case 4:
                excluirUsuario(arquivo);
                break;
            case 5:
                printf("Saindo...\n");
                break;
            default:
                printf("Opção inválida!\n");
        }

        fseek(arquivo, 0, SEEK_SET);
        
    } while (opcao != 5);

    fclose(arquivo);

    return 0;
}
