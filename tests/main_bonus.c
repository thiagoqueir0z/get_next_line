/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thiferre <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/07 11:35:56 by thiferre          #+#    #+#             */
/*   Updated: 2025/11/07 11:35:59 by thiferre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line_bonus.h"
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h> 
#include <unistd.h> 
#include <limits.h> // Para INT_MAX, usado como limite de FDs

// Nota: ft_strlen deve ser fornecido por get_next_line_utils.c e o header.

/**
 * @brief Executa o teste de leitura intercalada em múltiplos FDs.
 * @param fds Um array de descritores de arquivo abertos.
 * @param count O número de FDs válidos no array.
 * @param buffer_size O valor do BUFFER_SIZE.
 */
void	test_intercalated(int *fds, int count, int buffer_size)
{
	char	*line;
	int		active_fds;
	int		i;
	int		*line_counts;

	// Inicializa o contador de linhas para cada FD
	line_counts = (int *)calloc(count, sizeof(int));
	if (!line_counts)
	{
		perror("Erro na alocação de contadores");
		return ;
	}
	
	printf("\n--- Leitura Intercalada em %d FDs | BUFFER_SIZE = %d ---\n", 
		count, buffer_size);
	printf("----------------------------------------------------------\n");

	active_fds = count;
	while (active_fds > 0)
	{
		i = 0;
		while (i < count)
		{
			if (fds[i] != -1) // -1 indica que o FD já terminou
			{
				line = get_next_line(fds[i]);
				if (!line)
				{
					// Se GNL retorna NULL, este FD terminou (EOF ou Erro)
					printf("[FD %d] EOF/NULL. Encerrando leitura.\n", fds[i]);
					close(fds[i]);
					fds[i] = -1; // Marca como encerrado
					active_fds--;
				}
				else
				{
					line_counts[i]++;
					// Imprime de qual FD e qual linha estamos lendo
					printf("[FD %d | L%02d]: %s", 
						fds[i], line_counts[i], line);
					
					if (line[ft_strlen(line) - 1] != '\n')
						printf(" [EOF sem \\n]\n");
					
					free(line);
				}
			}
			i++;
		}
	}
	free(line_counts);
}

int	main(int argc, char **argv)
{
	int	fds[argc - 1]; // Array para armazenar os FDs
	int	i;
	
	// Lógica para detectar o BUFFER_SIZE usado na compilação
	#ifdef BUFFER_SIZE
		int buffer_size = BUFFER_SIZE;
	#else
		int buffer_size = 42; // Fallback
	#endif

	if (argc < 2)
	{
		printf("Uso: ./%s <arquivo1> <arquivo2> ...\n", argv[0]);
		printf("Para o bônus, são necessários múltiplos arquivos para leitura intercalada.\n");
		return (1);
	}
	
	// 1. Abre todos os arquivos
	i = 0;
	while (i < argc - 1)
	{
		fds[i] = open(argv[i + 1], O_RDONLY);
		if (fds[i] < 0)
		{
			perror("Erro ao abrir arquivo");
			// Fecha os FDs abertos até o momento, se houver
			for (int j = 0; j < i; j++) close(fds[j]); 
			return (1);
		}
		printf("Abriu FD %d para arquivo: %s\n", fds[i], argv[i + 1]);
		i++;
	}

	// 2. Executa o teste de leitura intercalada
	test_intercalated(fds, argc - 1, buffer_size);
	
	// 3. Teste de FDs Inválidos (mantido do main.c)
	// A GNL Bonus deve garantir que FDs fora do limite do array estático
	// ou negativos retornem NULL.
	printf("\n============================================\n");
	printf("TESTE DE ERRO CRÍTICO (FDs Inválidos) GNL BONUS\n");
	printf("============================================\n");
	
	char *line;
	line = get_next_line(INT_MAX); // FD muito alto (fora do limite do array)
	if (line == NULL)
		printf("✅ Teste 1: get_next_line(INT_MAX) retornou NULL (Correto).\n");
	else { free(line); printf("❌ Teste 1: get_next_line(INT_MAX) retornou uma linha.\n"); }

	line = get_next_line(-1); // FD Negativo
	if (line == NULL)
		printf("✅ Teste 2: get_next_line(-1) retornou NULL (Correto).\n");
	else { free(line); printf("❌ Teste 2: get_next_line(-1) retornou uma linha.\n"); }
	
	return (0);
}
