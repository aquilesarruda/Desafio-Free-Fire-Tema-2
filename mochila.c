#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/*
  Sistema de inventário: Mochila virtual de sobrevivência (Código da ilha – edição Free Fire)

  Funcionalidades:
  - Struct Item com nome, tipo e quantidade
  - Cadastro (até 10 itens), remoção, listagem e busca sequencial por nome
  - Interface simples em menu textual

  Observações de implementação:
  - Uso de fgets para ler strings com segurança e remoção do '\n' final
  - Uso de scanf para inteiros e limpeza do buffer de entrada após leituras
  - Após cada operação, a listagem completa é exibida (requisito funcional)
*/

#define MAX_ITENS 10
#define TAM_NOME 30
#define TAM_TIPO 20

typedef struct {
	char nome[TAM_NOME];
	char tipo[TAM_TIPO];
	int quantidade;
} Item;

/* Protótipos */
void limparBufferEntrada(void);
void lerLinha(char *destino, size_t tamanhoMaximo);
int buscarItemIndex(const Item inventario[], int numItens, const char *nome);
void inserirItem(Item inventario[], int *numItens);
void removerItem(Item inventario[], int *numItens);
void listarItens(const Item inventario[], int numItens);
void buscarItem(const Item inventario[], int numItens);

/*
  limpa qualquer caractere remanescente na entrada até a próxima quebra de linha ou EOF
*/
void limparBufferEntrada(void) {
	int c;
	while ((c = getchar()) != '\n' && c != EOF) {
		/* descarta */
	}
}

/*
  Lê uma linha segura para strings (inclui espaços) e remove o '\n' final, se presente.
*/
void lerLinha(char *destino, size_t tamanhoMaximo) {
	if (fgets(destino, (int)tamanhoMaximo, stdin) != NULL) {
		size_t len = strlen(destino);
		if (len > 0 && destino[len - 1] == '\n') {
			destino[len - 1] = '\0';
		}
	} else {
		/* Em caso de erro, garante string vazia */
		if (tamanhoMaximo > 0) destino[0] = '\0';
	}
}

/*
  Busca sequencial por nome no vetor 'inventario'.
  Retorna o índice do primeiro item encontrado, ou -1 se não existir.
*/
int buscarItemIndex(const Item inventario[], int numItens, const char *nome) {
	for (int i = 0; i < numItens; i++) {
		if (strcmp(inventario[i].nome, nome) == 0) {
			return i;
		}
	}
	return -1;
}

/*
  Cadastra um novo item, caso haja espaço disponível.
  Lê nome, tipo e quantidade do usuário e adiciona ao inventário.
*/
void inserirItem(Item inventario[], int *numItens) {
	if (*numItens >= MAX_ITENS) {
		printf("Mochila cheia! Você só pode carregar até %d itens.\n", MAX_ITENS);
		return;
	}

	Item novo;

	printf("Digite o nome do item: ");
	lerLinha(novo.nome, sizeof(novo.nome));
	if (novo.nome[0] == '\0') {
		printf("Nome inválido. Operação cancelada.\n");
		return;
	}

	printf("Digite o tipo do item (ex.: arma, municao, cura, ferramenta): ");
	lerLinha(novo.tipo, sizeof(novo.tipo));
	if (novo.tipo[0] == '\0') {
		printf("Tipo inválido. Operação cancelada.\n");
		return;
	}

	printf("Digite a quantidade: ");
	if (scanf("%d", &novo.quantidade) != 1) {
		printf("Entrada inválida para quantidade. Operação cancelada.\n");
		limparBufferEntrada();
		return;
	}
	limparBufferEntrada();

	if (novo.quantidade < 0) {
		printf("Quantidade não pode ser negativa. Operação cancelada.\n");
		return;
	}

	inventario[*numItens] = novo;
	(*numItens)++;
	printf("Item cadastrado com sucesso!\n");
}

/*
  Remove um item pelo nome. Se existir mais de um com o mesmo nome, remove o primeiro.
  Elementos seguintes são deslocados para fechar o espaço.
*/
void removerItem(Item inventario[], int *numItens) {
	if (*numItens == 0) {
		printf("A mochila está vazia. Nada para remover.\n");
		return;
	}

	char nomeBusca[TAM_NOME];
	printf("Digite o nome do item a remover: ");
	lerLinha(nomeBusca, sizeof(nomeBusca));

	int idx = buscarItemIndex(inventario, *numItens, nomeBusca);
	if (idx == -1) {
		printf("Item '%s' não encontrado.\n", nomeBusca);
		return;
	}

	for (int i = idx; i < *numItens - 1; i++) {
		inventario[i] = inventario[i + 1];
	}
	(*numItens)--;
	printf("Item removido com sucesso.\n");
}

/*
  Lista todos os itens do inventário com seus dados.
*/
void listarItens(const Item inventario[], int numItens) {
	printf("\n=== Itens na mochila (%d/%d) ===\n", numItens, MAX_ITENS);
	if (numItens == 0) {
		printf("(vazio)\n");
		printf("===============================\n\n");
		return;
	}

	for (int i = 0; i < numItens; i++) {
		printf("%d) Nome: %s | Tipo: %s | Quantidade: %d\n",
		       i + 1, inventario[i].nome, inventario[i].tipo, inventario[i].quantidade);
	}
	printf("===============================\n\n");
}

/*
  Busca um item pelo nome e exibe seus dados (se encontrado).
*/
void buscarItem(const Item inventario[], int numItens) {
	if (numItens == 0) {
		printf("A mochila está vazia.\n");
		return;
	}
	char nomeBusca[TAM_NOME];
	printf("Digite o nome do item a buscar: ");
	lerLinha(nomeBusca, sizeof(nomeBusca));

	int idx = buscarItemIndex(inventario, numItens, nomeBusca);
	if (idx == -1) {
		printf("Item '%s' não encontrado.\n", nomeBusca);
		return;
	}

	printf("Item encontrado:\n");
	printf("Nome: %s\nTipo: %s\nQuantidade: %d\n",
	       inventario[idx].nome, inventario[idx].tipo, inventario[idx].quantidade);
}

int main(void) {
	Item inventario[MAX_ITENS];
	int numItens = 0;
	int opcao;

	printf("==========================================\n");
	printf("  Mochila de Sobrevivência - Código da Ilha\n");
	printf("==========================================\n\n");

	while (1) {
		printf("Menu:\n");
		printf("1 - Cadastrar item\n");
		printf("2 - Remover item por nome\n");
		printf("3 - Listar itens\n");
		printf("4 - Buscar item por nome\n");
		printf("0 - Sair\n");
		printf("Escolha uma opção: ");

		if (scanf("%d", &opcao) != 1) {
			printf("Entrada inválida. Tente novamente.\n\n");
			limparBufferEntrada();
			continue;
		}
		limparBufferEntrada();

		switch (opcao) {
			case 1:
				inserirItem(inventario, &numItens);
				listarItens(inventario, numItens);
				break;
			case 2:
				removerItem(inventario, &numItens);
				listarItens(inventario, numItens);
				break;
			case 3:
				listarItens(inventario, numItens);
				break;
			case 4:
				buscarItem(inventario, numItens);
				listarItens(inventario, numItens);
				break;
			case 0:
				printf("Saindo... Boa sorte na ilha!\n");
				return 0;
			default:
				printf("Opção inválida. Tente novamente.\n\n");
		}
	}
}
