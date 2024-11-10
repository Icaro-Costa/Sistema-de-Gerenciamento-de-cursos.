#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_ALUNOS 100

typedef struct {
    int matricula;
    char nome[50];
    int sexo;
    int idade;
    int curso[4];
    int turno[4];
    int num_cursos;
    float mensalidade;
} Aluno;

Aluno alunos[MAX_ALUNOS];
int total_alunos = 0;

int ler_inteiro(const char *prompt, int min, int max) {
    int valor;
    char entrada[50]; 
    while (1) {
        printf("%s", prompt); 
        fgets(entrada, sizeof(entrada), stdin); 
       
        if (sscanf(entrada, "%d", &valor) == 1 && valor >= min && valor <= max) {
            return valor;
        }
        printf("Entrada invalida. Tente novamente.\n"); 
    }
}

float calcular_mensalidade(Aluno *aluno) {
    float total = 0;
    for (int i = 0; i < aluno->num_cursos; i++) {
        switch (aluno->curso[i]) {
            case 1:
                total += (aluno->turno[i] == 1) ? 360.00 : 420.00;
                break;
            case 2:
                total += (aluno->turno[i] == 1) ? 320.00 : 390.00;
                break;
            case 3:
                total += (aluno->turno[i] == 1) ? 290.00 : 310.00;
                break;
            case 4:
                total += (aluno->turno[i] == 1) ? 340.00 : 400.00;
                break;
        }
    }
    if (aluno->idade > 45) {
        total *= 0.85;
    }
    if (aluno->num_cursos > 1) {
        total *= 0.70;
    }
    return total;
}

void salvar_alunos() {
    FILE *arquivo;
    arquivo = fopen("alunos.txt", "w");

    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo para salvar os alunos.\n");
        return;
    }

    for (int i = 0; i < total_alunos; i++) {
        fprintf(arquivo, "Matricula: %d\n", alunos[i].matricula);
        fprintf(arquivo, "Nome: %s\n", alunos[i].nome);
        fprintf(arquivo, "Sexo: %s\n", alunos[i].sexo == 1 ? "Feminino" : "Masculino");
        fprintf(arquivo, "Idade: %d\n", alunos[i].idade);
        fprintf(arquivo, "Cursos:\n");
        for (int j = 0; j < alunos[i].num_cursos; j++) {
            fprintf(arquivo, "    Curso %d: %d\n", j + 1, alunos[i].curso[j]);
            fprintf(arquivo, "    Turno %d: %d\n", j + 1, alunos[i].turno[j]);
        }
        fprintf(arquivo, "Mensalidade: %.2f\n", alunos[i].mensalidade);
        fprintf(arquivo, "-------------------------\n"); 
    }

    fclose(arquivo); 
}

void carregar_alunos() {
    FILE *arquivo;
    arquivo = fopen("alunos.txt", "r");

    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo para carregar os alunos.\n");
        return;
    }

    while (fscanf(arquivo, "Matricula: %d\n", &alunos[total_alunos].matricula) == 1) {
        fscanf(arquivo, "Nome: %[^\n]\n", alunos[total_alunos].nome);
        char sexo[10];
        fscanf(arquivo, "Sexo: %[^\n]\n", sexo);
        alunos[total_alunos].sexo = (strcmp(sexo, "Feminino") == 0) ? 1 : 2;
        fscanf(arquivo, "Idade: %d\n", &alunos[total_alunos].idade);
        fscanf(arquivo, "Cursos:\n");
        while (1) {
            int curso, turno;
            if (fscanf(arquivo, "    Curso %*d: %d\n", &curso) != 1) break;
            fscanf(arquivo, "    Turno %*d: %d\n", &turno);
            alunos[total_alunos].curso[alunos[total_alunos].num_cursos] = curso;
            alunos[total_alunos].turno[alunos[total_alunos].num_cursos] = turno;
            alunos[total_alunos].num_cursos++;
        }
        fscanf(arquivo, "Mensalidade: %f\n", &alunos[total_alunos].mensalidade);
        fscanf(arquivo, "-------------------------\n"); 
        total_alunos++; 
    }

    fclose(arquivo); 
}

void cadastrar_aluno() {
    if (total_alunos >= MAX_ALUNOS) {
        printf("Limite de alunos atingido!\n");
        return;
    }

    Aluno aluno;
    int matricula;
    int matricula_existente;
    int mais_cursos;

    do {
        matricula = ler_inteiro("Matricula: ", 1, 999999);

        matricula_existente = 0;
        for (int i = 0; i < total_alunos; i++) {
            if (alunos[i].matricula == matricula) {
                matricula_existente = 1;
                break;
            }
        }

        if (matricula_existente) {
            printf("Matricula ja existe. Por favor, insira uma matricula diferente.\n");
        }

    } while (matricula_existente);

    aluno.matricula = matricula;

    printf("Nome: ");
    fgets(aluno.nome, sizeof(aluno.nome), stdin);
    aluno.nome[strcspn(aluno.nome, "\n")] = 0;

    aluno.sexo = ler_inteiro("Sexo (1-Feminino / 2-Masculino): ", 1, 2);

    aluno.idade = ler_inteiro("Idade: ", 0, 120);

    aluno.num_cursos = 0;
    int num_horarios_diferentes = 0;  // Reiniciando para cada aluno

    do {
        if (num_horarios_diferentes >= 2) {
            printf("O aluno já está cadastrado em dois horários diferentes. Não é possível cadastrar mais cursos.\n");
            break;
        }

        int curso, turno;
        curso = ler_inteiro("Curso (1-Seguranca de Informacao / 2-Java / 3-Arduino / 4-Linux): ", 1, 4);
        turno = ler_inteiro("Turno (1-Manha / 2-Noite): ", 1, 2);

        int curso_repetido = 0;
        for (int i = 0; i < aluno.num_cursos; i++) {
            if (aluno.curso[i] == curso && aluno.turno[i] == turno) {
                curso_repetido = 1;
                printf("O aluno já está cadastrado neste curso e turno.\n");
                break;
            }
        }

        if (!curso_repetido) {
            aluno.curso[aluno.num_cursos] = curso;
            aluno.turno[aluno.num_cursos] = turno;
            aluno.num_cursos++;
            num_horarios_diferentes++;
        }

        if (num_horarios_diferentes < 2) {
            mais_cursos = ler_inteiro("Deseja cadastrar outro curso (1-Sim / 2-Nao)? ", 1, 2);
        } else {
            mais_cursos = 2;  // Força sair do loop se já atingiu o limite de horários diferentes
        }

    } while (mais_cursos == 1);

    aluno.mensalidade = calcular_mensalidade(&aluno);

    if (aluno.num_cursos > 0) {
        alunos[total_alunos] = aluno;
        total_alunos++;
        printf("Aluno cadastrado com sucesso!\n");
    } else {
        printf("Nenhum curso foi cadastrado para o aluno.\n");
    }
}
void editar_aluno() {
    int matricula = ler_inteiro("Matricula do aluno a ser editado: ", 1, 999999);

    for (int i = 0; i < total_alunos; i++) {
        if (alunos[i].matricula == matricula) {
            Aluno *aluno = &alunos[i];
            printf("Dados do aluno:\n");
            printf("Matricula: %d\n", aluno->matricula);
            printf("Nome: %s\n", aluno->nome);
            printf("Sexo: %s\n", aluno->sexo == 1 ? "Feminino" : "Masculino");
            printf("Idade: %d\n", aluno->idade);
            printf("Cursos:\n");
            for (int j = 0; j < aluno->num_cursos; j++) {
                printf("    Curso %d: %s\n", j + 1,
                       aluno->curso[j] == 1 ? "Seguranca de Informacao" :
                       aluno->curso[j] == 2 ? "Java" :
                       aluno->curso[j] == 3 ? "Arduino" : "Linux");
                printf("    Turno %d: %s\n", j + 1,
                       aluno->turno[j] == 1 ? "Manha" : "Noite");
            }
            printf("Mensalidade: %.2f\n", aluno->mensalidade);
            printf("-------------------------\n");
            printf("Opcoes de edicao:\n");
            printf("1 - Nome\n");
            printf("2 - Sexo\n");
            printf("3 - Idade\n");
            printf("4 - Editar Horário de Curso\n");
            printf("0 - Cancelar\n");

            int opcao = ler_inteiro("Escolha o campo para editar: ", 0, 4);

            switch (opcao) {
                case 1:
                    printf("Nome atual: %s\n", aluno->nome);
                    printf("Novo nome: ");
                    fgets(aluno->nome, sizeof(aluno->nome), stdin);
                    aluno->nome[strcspn(aluno->nome, "\n")] = 0;
                    break;
                case 2:
                    aluno->sexo = ler_inteiro("Sexo (1-Feminino / 2-Masculino): ", 1, 2);
                    break;
                case 3:
                    aluno->idade = ler_inteiro("Idade: ", 0, 120);
                    break;
                case 4: {
                    int curso_editar;
                    curso_editar = ler_inteiro("Numero do curso a ser editado: ", 1, aluno->num_cursos);
                    int novo_turno;
                    novo_turno = ler_inteiro("Novo Turno (1-Manha / 2-Noite): ", 1, 2);

                    aluno->turno[curso_editar - 1] = novo_turno;
                    printf("Horário do curso editado com sucesso!\n");
                    break;
                }
                case 0:
                    printf("Edicao cancelada.\n");
                    return;
                default:
                    printf("Opcao invalida.\n");
                    return;
            }

            aluno->mensalidade = calcular_mensalidade(aluno);
            printf("Aluno editado com sucesso!\n");
            return;
        }
    }
    printf("Aluno nao encontrado!\n");
}

void remover_aluno() {
    int matricula = ler_inteiro("Matricula do aluno a ser removido: ", 1, 999999);
    int encontrado = 0;
    for (int i = 0; i < total_alunos; i++) {
        if (alunos[i].matricula == matricula) {
            for (int j = i; j < total_alunos - 1; j++) {
                alunos[j] = alunos[j + 1];
            }
            total_alunos--;
            printf("Aluno removido com sucesso!\n");
            encontrado = 1;
            break;
        }
    }
    if (!encontrado) {
        printf("Aluno nao encontrado!\n");
    }
}


void listar_alunos() {
    for (int i = 0; i < total_alunos; i++) {
        Aluno *aluno = &alunos[i];
        printf("-------------------------\n");
        printf("Matricula: %d\n", aluno->matricula);
        printf("Nome: %s\n", aluno->nome);
        printf("Sexo: %s\n", aluno->sexo == 1 ? "Feminino" : "Masculino");
        printf("Idade: %d\n", aluno->idade);
        printf("Cursos: ");
        for (int j = 0; j < aluno->num_cursos; j++) {
            printf("%s (%s)", aluno->curso[j] == 1 ? "Seguranca de Informacao" :
                               aluno->curso[j] == 2 ? "Java" :
                               aluno->curso[j] == 3 ? "Arduino" : "Linux",
                               aluno->turno[j] == 1 ? "Manha" : "Noite");
            if (j < aluno->num_cursos - 1) {
                printf(", ");
            }
        }
        printf("\nMensalidade: %.2f\n", aluno->mensalidade);
        printf("\n");
    }
}


void listar_alunos_por_sexo(int sexo) {
    int encontrou_alunos = 0;  
    
    for (int i = 0; i < total_alunos; i++) {
        if (alunos[i].sexo == sexo) {
            encontrou_alunos = 1;
            
            Aluno *aluno = &alunos[i];
            printf("-------------------------\n");
            printf("Matricula: %d\n", aluno->matricula);
            printf("Nome: %s\n", aluno->nome);
            printf("Sexo: %s\n", aluno->sexo == 1 ? "Feminino" : "Masculino");
            printf("Idade: %d\n", aluno->idade);
            printf("Cursos: ");
            for (int j = 0; j < aluno->num_cursos; j++) {
                printf("%s (%s)", aluno->curso[j] == 1 ? "Seguranca de Informacao" :
                                   aluno->curso[j] == 2 ? "Java" :
                                   aluno->curso[j] == 3 ? "Arduino" : "Linux",
                                   aluno->turno[j] == 1 ? "Manha" : "Noite");
                if (j < aluno->num_cursos - 1) {
                    printf(", ");
                }
            }
            printf("\nMensalidade: %.2f\n", aluno->mensalidade);
            printf("\n");
        }
    }
    
    if (!encontrou_alunos) {
        printf("-------------------------\n");   
        printf("Nenhum aluno cadstradado com esse sexo.\n");
    }
}

void listar_alunos_por_curso(int curso) {
    for (int i = 0; i < total_alunos; i++) {
        Aluno *aluno = &alunos[i];
        for (int j = 0; j < aluno->num_cursos; j++) {
            if (aluno->curso[j] == curso) {
                printf("-------------------------\n");
                printf("Matricula: %d\n", aluno->matricula);
                printf("Nome: %s\n", aluno->nome);
                printf("Sexo: %s\n", aluno->sexo == 1 ? "Feminino" : "Masculino");
                printf("Idade: %d\n", aluno->idade);
                printf("Cursos: ");
                for (int k = 0; k < aluno->num_cursos; k++) {
                    printf("%s (%s)", aluno->curso[k] == 1 ? "Seguranca de Informacao" :
                                       aluno->curso[k] == 2 ? "Java" :
                                       aluno->curso[k] == 3 ? "Arduino" : "Linux",
                                       aluno->turno[k] == 1 ? "Manha" : "Noite");
                    if (k < aluno->num_cursos - 1) {
                        printf(", ");
                    }
                }
                printf("\nMensalidade: %.2f\n", aluno->mensalidade);
                printf("\n");
                break;
            }
        }
    }
}

int main() {
    int opcao;
    carregar_alunos();
    do {
        printf("-------------------------\n");
        printf("TI Cursos\n");
        printf("1 - Cadastrar Aluno\n");
        printf("2 - Editar Aluno\n");
        printf("3 - Remover Aluno\n");
        printf("4 - Listagem Geral\n");
        printf("5 - Listagem por Curso\n");
        printf("6 - Listagem por Sexo\n");
        printf("0 - Sair\n");
        opcao = ler_inteiro("Escolha uma opcao: ", 0, 6);
        printf("-------------------------\n");
        switch (opcao) {
            case 1:
                cadastrar_aluno();
                salvar_alunos(); 
                break;
            case 2:
                editar_aluno();
                salvar_alunos(); //
                break;
            case 3:
                remover_aluno();
                salvar_alunos(); 
                break;
            case 4:
                listar_alunos();
                break;
            case 5: {
                int curso = ler_inteiro("Curso (1-Seguranca de Informacao / 2-Java / 3-Arduino / 4-Linux): ", 1, 4);
                listar_alunos_por_curso(curso);
                break;
            }
            case 6: {
                int sexo = ler_inteiro("Sexo (1-Feminino / 2-Masculino): ", 1, 2);
                listar_alunos_por_sexo(sexo);
                break;
            }
            case 0:
                printf("Saindo...\n");
                break;
            default:
                printf("Opcao invalida. Tente novamente.\n");
        }
    } while (opcao != 0);

    salvar_alunos();

    return 0;
}