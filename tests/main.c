/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thiferre <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/07 09:48:06 by thiferre          #+#    #+#             */
/*   Updated: 2025/11/07 11:20:14 by thiferre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h> 
#include <unistd.h> 

/**
 * @brief Realiza o teste de leitura em qualquer File Descriptor (FD).
 * @param fd O descritor de arquivo a ser lido (pode ser 0 para STDIN).
 * @param src_name Nome da fonte (para exibição no console).
 * @param buffer_size O valor do BUFFER_SIZE.
 */
void	test_fd(int fd, const char *src_name, int buffer_size)
{
	char	*line;
	int		count;

	printf("\n--- Leitura de FD %d (%s) | BUFFER_SIZE = %d ---\n", 
		fd, src_name, buffer_size);
	printf("--------------------------------------------------\n");

	count = 1;
	while (1)
	{
		line = get_next_line(fd);
		if (!line)
			break ;
			
		// Imprime a linha, mostrando o número da linha lida.
		printf("L%02d:\t%s", count++, line);
		
		// Verifica se a última linha lida é a do EOF e se não tem \n
		if (line[ft_strlen(line) - 1] != '\n')
			printf(" [EOF sem \\n - Tamanho: %d]\n", ft_strlen(line));
		
		free(line);
	}
	// Fecha o FD, exceto se for STDIN (FD 0)
	if (fd > 0)
		close(fd);
}

/**
 * @brief Testa o comportamento da GNL com FDs inválidos/arbitrários.
 * * Verifica o contrato: o valor de retorno deve ser NULL em caso de erro.
 */
void	test_invalid_fd(void)
{
	char	*line;

	printf("\n============================================\n");
	printf("TESTE DE ERRO CRÍTICO (FDs Inválidos)\n");
	printf("============================================\n");
	
	// Teste 1: FD Arbitrário Inválido (Ex: 42)
	line = get_next_line(42); 
	if (line == NULL)
		printf("✅ Teste 1: get_next_line(42) retornou NULL (Correto).\n");
	else
	{
		printf("❌ Teste 1: get_next_line(42) retornou uma linha ('%s').\n", line);
		free(line);
	}

	// Teste 2: FD Negativo Inválido (Ex: -1)
	line = get_next_line(-1);
	if (line == NULL)
		printf("✅ Teste 2: get_next_line(-1) retornou NULL (Correto).\n");
	else
	{
		printf("❌ Teste 2: get_next_line(-1) retornou uma linha ('%s').\n", line);
		free(line);
	}

	// Teste 3: Verificação de BUFFER_SIZE (Quando definido <= 0)
	printf("\nVerificação de Buffer Size:\n");
	# ifndef BUFFER_SIZE
	#  define BUFFER_SIZE 0 
	# endif
	if (BUFFER_SIZE <= 0)
		printf("✅ get_next_line() deve retornar NULL se BUFFER_SIZE <= 0 (Correto).\n");
	else
		printf("ℹ️  BUFFER_SIZE > 0 (%d): Teste de limite (boundary) ignorado.\n", BUFFER_SIZE);
	
	printf("\n");
}


int	main(int argc, char **argv)
{
	// Lógica para detectar o BUFFER_SIZE usado na compilação
	#ifdef BUFFER_SIZE
		int buffer_size = BUFFER_SIZE;
	#else
		int buffer_size = 4096; // Fallback se não for definido
	#endif

	if (argc > 1)
	{
		// --- Teste de Leitura de Arquivo (Usado pelo script) ---
		int fd = open(argv[1], O_RDONLY);
		if (fd < 0)
		{
			printf("Erro ao abrir arquivo: %s\n", argv[1]);
			return (1);
		}
		test_fd(fd, argv[1], buffer_size);
	}
	else // argc == 1
	{
		// --- Teste de Leitura de STDIN (Usado pelo script) ---
		printf("--- Testando STDIN (Entrada Padrão) ---\n");
		test_fd(0, "STDIN", buffer_size);
		printf(">>> Teste de STDIN concluído.\n");
	}

	// --- Teste de Estresse de Erro de FD ---
	test_invalid_fd();
	
	return (0);
}
