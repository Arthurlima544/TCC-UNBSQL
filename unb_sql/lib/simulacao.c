#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_TOKENS 100
#define MAX_TOKEN_LEN 64
#define MAX_IDENT_LEN 64
#define MAX_COLUMNS 20
#define MAX_VALUES 20
#define MAX_VALUE_LEN 64

// === Tokens ===
// TODO add others ;.()[]
typedef enum {
    TK_INSERE,       // INSERE
    TK_EM,           // EM
    TK_VALORES,      // VALORES
    TK_IDENT,        // nome_da_tabela, colunas
    TK_STRING,       // 'texto'
    TK_NUM,          // número literal
    TK_PT_VIRG,      // ;
    TK_VIRG,         // ,
    TK_ABRE_PAR,     // (
    TK_FECHA_PAR,    // )
    TK_CRIAR,        // CRIAR
    TK_TABELA,       // TABELA
    TK_SELECIONE,    // SELECIONE
    TK_DE,           // DE
    TK_ONDE,         // ONDE
    TK_CONTEM,       // CONTEM
    TK_AGRUPADO,     // AGRUPADO
    TK_POR,          // POR
    TK_ORDENADO,     // ORDENADO
    TK_DESCENDENTE,  // DESCENDENTE
    TK_COMO,         // COMO
    TK_CHAVE,        // CHAVE
    TK_PRIMARIA,     // PRIMARIA
    TK_ESTRANGEIRA,  // ESTRANGEIRA
    TK_REFERENCIA,   // REFERENCIA
    TK_EOF,
    TK_UNKNOWN
} TokenType;


typedef struct {
    TokenType type;
    char text[MAX_TOKEN_LEN];
} Token;

// === Analisador Léxico ===
typedef struct {
    const char *src;
    int pos;
    Token tokens[MAX_TOKENS];
    int ntokens;
} Lexer;


int isIdentChar(char c) {
    return isalnum(c) || c == '_';
}

void addToken(Lexer *lexer, TokenType type, const char *start, int length) {
    if (lexer->ntokens >= MAX_TOKENS) {
        fprintf(stderr, "Erro: muitos tokens.\n");
        exit(1);
    }
    Token *t = &lexer->tokens[lexer->ntokens++];
    t->type = type;
    if (length >= MAX_TOKEN_LEN) length = MAX_TOKEN_LEN -1;
    strncpy(t->text, start, length);
    t->text[length] = 0;
}

void lexer_skip_space(Lexer *lexer) {
    while (lexer->src[lexer->pos] && isspace(lexer->src[lexer->pos]))
        lexer->pos++;
}

int lexer_match_keyword(const char *start, int length, const char *keyword) {
    if (length != (int)strlen(keyword)) return 0;
    for (int i = 0; i < length; i++) {
        if (toupper(start[i]) != keyword[i])
            return 0;
    }
    return 1;
}

void lexer_tokenize(Lexer *lexer) {
    lexer->ntokens = 0;
    lexer_skip_space(lexer);
    while (lexer->src[lexer->pos]) {
        char c = lexer->src[lexer->pos];
        if (isspace(c)) {
            lexer_skip_space(lexer);
            continue;
        }
        if (isalpha(c)) {
            int start = lexer->pos;
            while (isIdentChar(lexer->src[lexer->pos])) lexer->pos++;
            int len = lexer->pos - start;
            const char *str = &lexer->src[start];
            // Verifica palavras reservadas
            if (lexer_match_keyword(str,len,"INSERE"))           addToken(lexer,TK_INSERE,str,len);
            else if (lexer_match_keyword(str,len,"EM"))          addToken(lexer,TK_EM,str,len);
            else if (lexer_match_keyword(str,len,"VALORES"))     addToken(lexer,TK_VALORES,str,len);
            else if (lexer_match_keyword(str,len,"CRIAR"))       addToken(lexer,TK_CRIAR,str,len);
            else if (lexer_match_keyword(str,len,"TABELA"))      addToken(lexer,TK_TABELA,str,len);
            else if (lexer_match_keyword(str,len,"SELECIONE"))   addToken(lexer,TK_SELECIONE,str,len);
            else if (lexer_match_keyword(str,len,"DE"))          addToken(lexer,TK_DE,str,len);
            else if (lexer_match_keyword(str,len,"ONDE"))        addToken(lexer,TK_ONDE,str,len);
            else if (lexer_match_keyword(str,len,"CONTEM"))      addToken(lexer,TK_CONTEM,str,len);
            else if (lexer_match_keyword(str,len,"AGRUPADO"))    addToken(lexer,TK_AGRUPADO,str,len);
            else if (lexer_match_keyword(str,len,"POR"))         addToken(lexer,TK_POR,str,len);
            else if (lexer_match_keyword(str,len,"ORDENADO"))    addToken(lexer,TK_ORDENADO,str,len);
            else if (lexer_match_keyword(str,len,"DESCENDENTE")) addToken(lexer,TK_DESCENDENTE,str,len);
            else if (lexer_match_keyword(str,len,"COMO"))        addToken(lexer,TK_COMO,str,len);
            else if (lexer_match_keyword(str,len,"CHAVE"))       addToken(lexer,TK_CHAVE,str,len);
            else if (lexer_match_keyword(str,len,"PRIMARIA"))    addToken(lexer,TK_PRIMARIA,str,len);
            else if (lexer_match_keyword(str,len,"ESTRANGEIRA")) addToken(lexer,TK_ESTRANGEIRA,str,len);
            else if (lexer_match_keyword(str,len,"REFERENCIA"))  addToken(lexer,TK_REFERENCIA,str,len);
            else addToken(lexer,TK_IDENT,str,len);  
               
        } else if (c == '\'') { // string literal
            int start = lexer->pos+1;
            lexer->pos++;
            while (lexer->src[lexer->pos] && lexer->src[lexer->pos] != '\'') lexer->pos++;
            if (lexer->src[lexer->pos] != '\'') {
                fprintf(stderr,"Erro: string literal sem fechamento.\n");
                exit(1);
            }
            int len = lexer->pos - start;
            addToken(lexer,TK_STRING,&lexer->src[start],len);
            lexer->pos++; // pular o '
        } else if (isdigit(c)) {
            int start = lexer->pos;
            while (isdigit(lexer->src[lexer->pos])) lexer->pos++;
            int len = lexer->pos - start;
            addToken(lexer,TK_NUM,&lexer->src[start],len);
        } else {
            switch(c) {
                case ';': addToken(lexer,TK_PT_VIRG,";",1); lexer->pos++; break;
                case ',': addToken(lexer,TK_VIRG,",",1); lexer->pos++; break;
                case '(': addToken(lexer,TK_ABRE_PAR,"(",1); lexer->pos++; break;
                case ')': addToken(lexer,TK_FECHA_PAR,")",1); lexer->pos++; break;
                default:
                    fprintf(stderr,"Erro: caractere desconhecido '%c'\n",c);
                    exit(1);
            }
        }
        lexer_skip_space(lexer);
    }
    addToken(lexer, TK_EOF, "", 0);
}

// === Parser ===
typedef struct {
    Token *tokens;
    int pos;
} Parser;

typedef struct {
    char tabela[MAX_IDENT_LEN];
    int n_colunas;
    char colunas[MAX_COLUMNS][MAX_IDENT_LEN];
    int n_valores;
    char valores[MAX_VALUES][MAX_VALUE_LEN];
} ASTInsert;

typedef struct {
    char nome_coluna[MAX_IDENT_LEN];
    char tipo_dado[MAX_IDENT_LEN];
    int eh_chave_primaria; // 0 ou 1
} ColunaTabela;

typedef struct {
    char coluna_fk[MAX_IDENT_LEN];
    char tabela_referencia[MAX_IDENT_LEN];
    char coluna_referencia[MAX_IDENT_LEN];
} ChaveEstrangeira;

typedef struct {
    char tabela[MAX_IDENT_LEN];
    int n_colunas;
    ColunaTabela colunas[MAX_COLUMNS];
    int n_chaves_estrangeiras;
    ChaveEstrangeira chaves_estrangeiras[MAX_COLUMNS];
} ASTCreateTable;

typedef struct {
    char nome_coluna[MAX_IDENT_LEN];
    int tem_agregacao; // 0 ou 1
    char agregacao[MAX_IDENT_LEN]; // ex: SOMA, MEDIA etc
    char alias[MAX_IDENT_LEN]; // texto depois do COMO, vazio se não usar
} ColunaSelect;

typedef struct {
    ColunaSelect colunas[MAX_COLUMNS];
    int n_colunas;

    char tabela[MAX_IDENT_LEN];

    // Condição simples para WHERE: coluna CONTEM 'string'
    int tem_where;
    char where_coluna[MAX_IDENT_LEN];
    char where_valor[MAX_VALUE_LEN];

    // Group by: lista de colunas
    int n_groupby;
    char groupby_cols[MAX_COLUMNS][MAX_IDENT_LEN];

    // Order by: lista de colunas + opção descendente
    int n_orderby;
    char orderby_cols[MAX_COLUMNS][MAX_IDENT_LEN];
    int orderby_descendente[MAX_COLUMNS]; // 0 para asc, 1 para desc
} ASTSelect;

Token* parser_peek(Parser *p) {
    return &p->tokens[p->pos];
}
Token* parser_consume(Parser *p, TokenType expected_type) {
    Token *t = parser_peek(p);
    if (t->type != expected_type) {
        fprintf(stderr,"Erro sintático: esperado token %d mas encontrou '%s'\n", expected_type, t->text);
        exit(1);
    }
    p->pos++;
    return t;
}

void parse_ident_list(Parser *p, char colunas[][MAX_IDENT_LEN], int *count) {
    *count = 0;
    parser_consume(p, TK_ABRE_PAR);
    Token *t = parser_peek(p);
    while (1) {
        if (t->type != TK_IDENT) {
            fprintf(stderr, "Erro sintático: esperado identificador na lista de colunas.\n");
            exit(1);
        }
        strncpy(colunas[*count], t->text, MAX_IDENT_LEN);
        (*count)++;
        p->pos++;
        
        t = parser_peek(p);
        if (t->type == TK_VIRG) { 
            p->pos++; // consumir ,
            t = parser_peek(p);
            continue;
        } else if (t->type == TK_FECHA_PAR) {
            p->pos++; // consumir )
            break;
        } else {
            fprintf(stderr, "Erro sintático: esperado ',' ou ')' na lista de colunas.\n");
            exit(1);
        }
    }
}

void parse_valores_list(Parser *p, char valores[][MAX_VALUE_LEN], int *count) {
    *count = 0;
    parser_consume(p, TK_ABRE_PAR);
    Token *t = parser_peek(p);
    while (1) {
        if (t->type != TK_STRING && t->type != TK_NUM) {
            fprintf(stderr, "Erro sintático: esperado string ou número na lista de valores.\n");
            exit(1);
        }
        // Copia valor incluindo aspas se for string
        if (t->type == TK_STRING) {
            snprintf(valores[*count], MAX_VALUE_LEN, "'%s'", t->text);
        } else {
            strncpy(valores[*count], t->text, MAX_VALUE_LEN);
        }
        (*count)++;
        p->pos++;
        
        t = parser_peek(p);
        if (t->type == TK_VIRG) {
            p->pos++; // consumir ,
            t = parser_peek(p);
            continue;
        } else if (t->type == TK_FECHA_PAR) {
            p->pos++; // consumir )
            break;
        } else {
            fprintf(stderr, "Erro sintático: esperado ',' ou ')' na lista de valores.\n");
            exit(1);
        }
    }
}

void parse_insert(Parser *p, ASTInsert *ast) {
    Token *t = parser_peek(p);
    if (t->type != TK_INSERE) {
        fprintf(stderr, "Erro sintático: esperado 'INSERE' no começo.\n");
        exit(1);
    }
    parser_consume(p, TK_INSERE);
    parser_consume(p, TK_EM);

    // Nome da tabela
    t = parser_peek(p);
    if (t->type != TK_IDENT) {
        fprintf(stderr, "Erro sintático: esperado nome da tabela.\n");
        exit(1);
    }
    strncpy(ast->tabela, t->text, MAX_IDENT_LEN);
    p->pos++;

    parse_ident_list(p, ast->colunas, &ast->n_colunas);

    t = parser_peek(p);
    if (t->type != TK_VALORES) {
        fprintf(stderr, "Erro sintático: esperado 'VALORES'.\n");
        exit(1);
    }
    parser_consume(p, TK_VALORES);

    parse_valores_list(p, ast->valores, &ast->n_valores);

    // Verifica que há ponto e vírgula no final
    t = parser_peek(p);
    if (t->type != TK_PT_VIRG) {
        fprintf(stderr, "Erro sintático: esperado ';' no final.\n");
        exit(1);
    }
    parser_consume(p, TK_PT_VIRG);
}



// AUX for data types
typedef struct {
    char nome_sql[MAX_IDENT_LEN]; // ex: INT, NUMERIC(5,2), VARCHAR(100), DATETIME, BOOLEAN
} TipoDado;

// Função para parsear tipo de dado (e parâmetros, ex NUMERICO(M,D))
void parse_tipo_dado(Parser *p, TipoDado *tipo) {
    Token *t = parser_peek(p);
    if (t->type != TK_IDENT) {
        fprintf(stderr, "Esperado tipo de dado\n");
        exit(1);
    }

    // Convertemos para maiúsculo para facilitar comparações
    char tipo_upper[MAX_IDENT_LEN];
    int i;
    for (i = 0; t->text[i] && i < MAX_IDENT_LEN-1; i++) {
        tipo_upper[i] = toupper(t->text[i]);
    }
    tipo_upper[i] = 0;

    if (strcmp(tipo_upper, "INTEIRO") == 0) {
        strcpy(tipo->nome_sql, "INT");
        parser_consume(p, TK_IDENT);
    } else if (strcmp(tipo_upper, "NUMERICO") == 0) {
        parser_consume(p, TK_IDENT);

        parser_consume(p, TK_ABRE_PAR);

        Token *m = parser_peek(p);
        if (m->type != TK_NUM) {
            fprintf(stderr, "Esperado parâmetro M (número) em NUMERICO(M,D)\n");
            exit(1);
        }
        char m_val[16];
        strncpy(m_val, m->text, sizeof(m_val));
        parser_consume(p, TK_NUM);

        parser_consume(p, TK_VIRG);

        Token *d = parser_peek(p);
        if (d->type != TK_NUM) {
            fprintf(stderr, "Esperado parâmetro D (número) em NUMERICO(M,D)\n");
            exit(1);
        }
        char d_val[16];
        strncpy(d_val, d->text, sizeof(d_val));
        parser_consume(p, TK_NUM);

        parser_consume(p, TK_FECHA_PAR);

        snprintf(tipo->nome_sql, MAX_IDENT_LEN, "NUMERIC(%s,%s)", m_val, d_val);

    } else if (strcmp(tipo_upper, "TEXTO") == 0) {
        // Para simplicidade geramos VARCHAR(255)
        strcpy(tipo->nome_sql, "VARCHAR(255)");
        parser_consume(p, TK_IDENT);
    } else if (strcmp(tipo_upper, "DATAHORA") == 0) {
        strcpy(tipo->nome_sql, "DATETIME");
        parser_consume(p, TK_IDENT);
    } else if (strcmp(tipo_upper, "BOLEANO") == 0) {
        // Mapeia para BOOLEAN ou TINYINT(1)
        strcpy(tipo->nome_sql, "BOOLEAN");
        parser_consume(p, TK_IDENT);
    } else if (strcmp(tipo_upper, "ENUMERACAO") == 0) {
        // Só ENUM simples, sem parâmetro aqui (simplificado)
        strcpy(tipo->nome_sql, "ENUM");
        parser_consume(p, TK_IDENT);
    } else if (strcmp(tipo_upper, "NULO") == 0) {
        // NULL como tipo - no SQL é usado para valores nulos, mas aceitamos aqui
        strcpy(tipo->nome_sql, "NULL");
        parser_consume(p, TK_IDENT);
    } else {
        fprintf(stderr, "Tipo de dado desconhecido: %s\n", t->text);
        exit(1);
    }
}
// Parser Create table
void parse_create_table(Parser *p, ASTCreateTable *ast) {
    parser_consume(p, TK_CRIAR);
    parser_consume(p, TK_TABELA);

    Token *t = parser_peek(p);
    if (t->type != TK_IDENT) {
        fprintf(stderr, "Esperado nome da tabela para CREATE\n");
        exit(1);
    }
    strncpy(ast->tabela, t->text, MAX_IDENT_LEN);
    p->pos++;

    parser_consume(p, TK_ABRE_PAR);

    ast->n_colunas = 0;
    ast->n_chaves_estrangeiras = 0;

    while (1) {
        t = parser_peek(p);
        if (t->type == TK_CHAVE) {
            // Pode ser CHAVE ESTRANGEIRA (...)
            parser_consume(p, TK_CHAVE);
            t = parser_peek(p);
            if (t->type == TK_PRIMARIA) {
                // CHAVE PRIMARIA como separado nao previsto no seu exemplo
                fprintf(stderr, "Erro sintático: CHAVE PRIMARIA isolada não suportada, use CHAVE PRIMARIA após coluna\n");
                exit(1);
            }
            else if (t->type == TK_ESTRANGEIRA) {
                parser_consume(p, TK_ESTRANGEIRA);

                parser_consume(p, TK_ABRE_PAR);
                t = parser_peek(p);
                if (t->type != TK_IDENT) {
                    fprintf(stderr, "Erro sintático: esperado nome da coluna para chave estrangeira\n");
                    exit(1);
                }
                strncpy(ast->chaves_estrangeiras[ast->n_chaves_estrangeiras].coluna_fk, t->text, MAX_IDENT_LEN);
                p->pos++;
                parser_consume(p, TK_FECHA_PAR);

                if (parser_peek(p)->type != TK_REFERENCIA) {
                    fprintf(stderr, "Esperado REFERENCIA após chave estrangeira\n");
                    exit(1);
                }
                parser_consume(p, TK_REFERENCIA);

                // outra_tabela(atributo_referenciado)
                t = parser_peek(p);
                if (t->type != TK_IDENT) {
                    fprintf(stderr,"Esperado nome da tabela referência\n");
                    exit(1);
                }
                strncpy(ast->chaves_estrangeiras[ast->n_chaves_estrangeiras].tabela_referencia, t->text, MAX_IDENT_LEN);
                p->pos++;

                parser_consume(p, TK_ABRE_PAR);

                t = parser_peek(p);
                if (t->type != TK_IDENT) {
                    fprintf(stderr, "Esperado atributo referencia na chave estrangeira\n");
                    exit(1);
                }
                strncpy(ast->chaves_estrangeiras[ast->n_chaves_estrangeiras].coluna_referencia, t->text, MAX_IDENT_LEN);
                p->pos++;

                parser_consume(p, TK_FECHA_PAR);

                ast->n_chaves_estrangeiras++;

            } else {
                fprintf(stderr, "Erro sintático: esperado PRIMARIA ou ESTRANGEIRA após CHAVE\n");
                exit(1);
            }
        }
        else if (t->type == TK_IDENT) {
            // coluna1 tipo_de_dado CHAVE PRIMARIA, ou coluna2 tipo_de_dado, etc.
            if (ast->n_colunas >= MAX_COLUMNS) {
                fprintf(stderr,"Erro: muitas colunas na tabela\n");
                exit(1);
            }
            ColunaTabela *col = &ast->colunas[ast->n_colunas];
            
            // Nome coluna
            strncpy(col->nome_coluna, t->text, MAX_IDENT_LEN);
            p->pos++;

            // Tipo de dado
            TipoDado tipo;
            parse_tipo_dado(p, &tipo);
            strncpy(col->tipo_dado, tipo.nome_sql, MAX_IDENT_LEN);

            // Verificar se há CHAVE PRIMARIA após isso
            col->eh_chave_primaria = 0;

            t = parser_peek(p);
            if (t->type == TK_CHAVE) {
                parser_consume(p, TK_CHAVE);
                t = parser_peek(p);
                if (t->type == TK_PRIMARIA) {
                    parser_consume(p, TK_PRIMARIA);
                    col->eh_chave_primaria = 1;
                } else {
                    fprintf(stderr, "Esperado PRIMARIA depois de CHAVE na coluna\n");
                    exit(1);
                }
            }

            ast->n_colunas++;

        } else {
            fprintf(stderr, "Token inesperado em definição da tabela: '%s'\n", t->text);
            exit(1);
        }

        // Depois espera vírgula ou fechar parênteses
        t = parser_peek(p);
        if (t->type == TK_VIRG) {
            parser_consume(p, TK_VIRG);
        } else if (t->type == TK_FECHA_PAR) {
            parser_consume(p, TK_FECHA_PAR);
            break;
        } else {
            fprintf(stderr, "Esperado ',' ou ')' na definição da tabela, encontrado '%s'\n", t->text);
            exit(1);
        }
    }

    // No final ';'
    t = parser_peek(p);
    if (t->type != TK_PT_VIRG) {
        fprintf(stderr, "Esperado ';' após CREATE TABLE\n");
        exit(1);
    }
    parser_consume(p, TK_PT_VIRG);
}

// Parser SELECT
void parse_select(Parser *p, ASTSelect *ast) {
    parser_consume(p, TK_SELECIONE);
    ast->n_colunas = 0;

    // Listagem colunas (com agregação e alias opcional)
    Token *t = parser_peek(p);
    while (1) {
        if (ast->n_colunas >= MAX_COLUMNS) {
            fprintf(stderr, "Erro: muitas colunas no SELECT\n");
            exit(1);
        }
        ColunaSelect *col = &ast->colunas[ast->n_colunas];

        // Inicializa
        col->tem_agregacao = 0;
        col->alias[0] = 0;

        // Pode ser IDENT (simples) ou AGREGACAO(IDENT)
        if (t->type == TK_IDENT) {
            strncpy(col->nome_coluna, t->text, MAX_IDENT_LEN);
            p->pos++;
        } else if (t->type == TK_IDENT || t->type == TK_INSERE) {
            // Geralmente o agregador é tipo SOMA, MEDIA, vamos aceitar todas como TK_IDENT e interpretar aqui:
            // Para simplicidade, se token for nome e próximo é ( identificador ')', é agregação.

            // Isso aqui cobrimos o caso agregação do jeito a seguir
            fprintf(stderr,"Erro sintático: esperado nome da coluna ou agregação\n");
            exit(1);
        } else {
            // Pode ser nome agregação (SOMA, MEDIA etc)
            if (t->type != TK_IDENT) {
                fprintf(stderr,"Erro sintático: esperado nome ou agregação, encontrado '%s'\n", t->text);
                exit(1);
            }
            // Not really expecting here.
        }

        t = parser_peek(p);

        // Se próximo for abre parênteses é agregação
        if (t->type == TK_ABRE_PAR) {
            // Em comando exemplo temos SOMA(matricula)
            strncpy(col->agregacao, col->nome_coluna, MAX_IDENT_LEN);
            col->tem_agregacao = 1;

            parser_consume(p, TK_ABRE_PAR);

            t = parser_peek(p);
            if (t->type != TK_IDENT) {
                fprintf(stderr,"Erro sintático: esperado nome de coluna na agregação\n");
                exit(1);
            }
            strncpy(col->nome_coluna, t->text, MAX_IDENT_LEN);
            parser_consume(p, TK_IDENT);

            parser_consume(p, TK_FECHA_PAR);

            t = parser_peek(p);
        }

        // Verifica alias "COMO"
        if (t->type == TK_COMO) {
            parser_consume(p, TK_COMO);
            t = parser_peek(p);
            if (t->type != TK_IDENT) {
                fprintf(stderr,"Erro sintático: esperado alias após COMO\n");
                exit(1);
            }
            strncpy(col->alias, t->text, MAX_IDENT_LEN);
            parser_consume(p, TK_IDENT);
            t = parser_peek(p);
        }

        ast->n_colunas++;

        if (t->type == TK_VIRG) {
            parser_consume(p, TK_VIRG);
            t = parser_peek(p);
            continue;
        } else {
            break;
        }
    }

    // Espera DE tabela
    if (parser_peek(p)->type != TK_DE) {
        fprintf(stderr, "Esperado DE após colunas no SELECT\n");
        exit(1);
    }
    parser_consume(p, TK_DE);

    t = parser_peek(p);
    if (t->type != TK_IDENT) {
        fprintf(stderr,"Esperado nome da tabela depois de DE\n");
        exit(1);
    }
    strncpy(ast->tabela, t->text, MAX_IDENT_LEN);
    parser_consume(p, TK_IDENT);

    // Verifica presença condicional ONDE
    ast->tem_where = 0;
    if (parser_peek(p)->type == TK_ONDE) {
        ast->tem_where = 1;
        parser_consume(p, TK_ONDE);
        t = parser_peek(p);

        if (t->type != TK_IDENT) {
            fprintf(stderr, "Esperado coluna na cláusula ONDE\n");
            exit(1);
        }
        strncpy(ast->where_coluna, t->text, MAX_IDENT_LEN);
        parser_consume(p, TK_IDENT);

        // Esperar CONTEM
        if (parser_peek(p)->type != TK_CONTEM) {
            fprintf(stderr,"Esperado CONTEM na cláusula ONDE\n");
            exit(1);
        }
        parser_consume(p, TK_CONTEM);

        // Esperar string literal
        t = parser_peek(p);
        if (t->type != TK_STRING) {
            fprintf(stderr,"Esperado string após CONTEM na cláusula ONDE\n");
            exit(1);
        }
        snprintf(ast->where_valor, MAX_VALUE_LEN, "'%s'", t->text);
        parser_consume(p, TK_STRING);
    }

    // AGRUPADO POR
    ast->n_groupby = 0;
    if (parser_peek(p)->type == TK_AGRUPADO) {
        parser_consume(p, TK_AGRUPADO);
        if (parser_peek(p)->type != TK_POR) {
            fprintf(stderr, "Esperado POR após AGRUPADO\n");
            exit(1);
        }
        parser_consume(p, TK_POR);

        // Lista de colunas agrupadas
        while (1) {
            if (ast->n_groupby >= MAX_COLUMNS) {
                fprintf(stderr,"Erro: muitas colunas em GROUP BY\n");
                exit(1);
            }
            t = parser_peek(p);
            if (t->type != TK_IDENT) {
                fprintf(stderr, "Esperado coluna após AGRUPADO POR\n");
                exit(1);
            }
            strncpy(ast->groupby_cols[ast->n_groupby], t->text, MAX_IDENT_LEN);
            ast->n_groupby++;
            parser_consume(p, TK_IDENT);

            t = parser_peek(p);
            if (t->type == TK_VIRG) {
                parser_consume(p, TK_VIRG);
                continue;
            } else {
                break;
            }
        }
    }

    // ORDENADO POR
    ast->n_orderby = 0;
    if (parser_peek(p)->type == TK_ORDENADO) {
        parser_consume(p, TK_ORDENADO);
        if (parser_peek(p)->type != TK_POR) {
            fprintf(stderr, "Esperado POR após ORDENADO\n");
            exit(1);
        }
        parser_consume(p, TK_POR);

        while (1) {
            if (ast->n_orderby >= MAX_COLUMNS) {
                fprintf(stderr,"Erro: muitas colunas em ORDER BY\n");
                exit(1);
            }
            t = parser_peek(p);
            if (t->type != TK_IDENT) {
                fprintf(stderr,"Esperado coluna após ORDENADO POR\n");
                exit(1);
            }
            strncpy(ast->orderby_cols[ast->n_orderby], t->text, MAX_IDENT_LEN);
            parser_consume(p, TK_IDENT);

            ast->orderby_descendente[ast->n_orderby] = 0;

            t = parser_peek(p);
            if (t->type == TK_DESCENDENTE) {
                ast->orderby_descendente[ast->n_orderby] = 1;
                parser_consume(p, TK_DESCENDENTE);
            }

            ast->n_orderby++;

            t = parser_peek(p);
            if (t->type == TK_VIRG) {
                parser_consume(p, TK_VIRG);
                continue;
            } else {
                break;
            }
        }
    }

    // Espera ';'
    t = parser_peek(p);
    if (t->type != TK_PT_VIRG) {
        fprintf(stderr, "Esperado ';' no final do SELECT\n");
        exit(1);
    }
    parser_consume(p, TK_PT_VIRG);
}


// Análise semântica simples: checar se quantidades de colunas e valores são iguais
void semantic_check(ASTInsert *ast) {
    if (ast->n_colunas != ast->n_valores) {
        fprintf(stderr, "Erro semântico: número de colunas e valores não coincidem (%d colunas x %d valores)\n",
                ast->n_colunas, ast->n_valores);
        exit(1);
    }
}

// Geração de código intermediário (simplesmente estrutura armazenada em ASTInsert)

void optimize(ASTInsert *ast) {
    // Nenhuma otimização aplicada
}

// Geração de SQL final (código de máquina neste contexto)
void generate_insert_sql(ASTInsert *ast, char *output, int maxlen) {
    int n = snprintf(output, maxlen, "INSERT INTO %s (", ast->tabela);
    for (int i = 0; i < ast->n_colunas; i++) {
        int rem = maxlen - n;
        if (i > 0) n += snprintf(output + n, rem, ", ");
        else n += snprintf(output + n, rem, "");
        rem = maxlen - n;
        n += snprintf(output + n, rem, "%s", ast->colunas[i]);
    }
    int rem = maxlen - n;
    n += snprintf(output + n, rem, ") VALUES (");
    for (int i = 0; i < ast->n_valores; i++) {
        rem = maxlen - n;
        if (i > 0) n += snprintf(output + n, rem, ", ");
        else n += snprintf(output + n, rem, "");
        rem = maxlen - n;
        n += snprintf(output + n, rem, "%s", ast->valores[i]);
    }
    rem = maxlen - n;
    n += snprintf(output + n, rem, ");");
}

void generate_create_sql(ASTCreateTable *ast, char *output, int maxlen) {
    int n = snprintf(output, maxlen, "CREATE TABLE %s (\n", ast->tabela);
    
    for (int i = 0; i < ast->n_colunas; i++) {
        ColunaTabela *col = &ast->colunas[i];
        int rem = maxlen - n;
        n += snprintf(output + n, rem, "    %s %s", col->nome_coluna, col->tipo_dado);
        if (col->eh_chave_primaria) {
            rem = maxlen - n;
            n += snprintf(output + n, rem, " PRIMARY KEY");
        }
        // Se não for última coluna, vírgula será colocada abaixo
        if (i < ast->n_colunas -1 || ast->n_chaves_estrangeiras > 0) {
            rem = maxlen - n;
            n += snprintf(output + n, rem, ",\n");
        } else {
            rem = maxlen - n;
            n += snprintf(output + n, rem, "\n");
        }
    }   
    int rem = maxlen - n;
    for (int i = 0; i < ast->n_chaves_estrangeiras; i++) {
        ChaveEstrangeira *fk = &ast->chaves_estrangeiras[i];
        int rem = maxlen - n;
        n += snprintf(output + n, rem, 
            "    FOREIGN KEY (%s) REFERENCES %s(%s)",
            fk->coluna_fk,
            fk->tabela_referencia,
            fk->coluna_referencia);
        // Se não for última chave estrangeira, vírgula + \n
        if (i < ast->n_chaves_estrangeiras -1) {
            rem = maxlen - n;
            n += snprintf(output + n, rem, ",\n");
        } else {
            rem = maxlen - n;
            n += snprintf(output + n, rem, "\n");
        }
    }
    rem = maxlen - n;
    n += snprintf(output + n, rem, ");");
}

void generate_select_sql(ASTSelect *ast, char *output, int maxlen) {
    int n = snprintf(output, maxlen, "SELECT ");

    for (int i = 0; i < ast->n_colunas; i++) {
        ColunaSelect *col = &ast->colunas[i];
        int rem = maxlen - n;
        if (i > 0) n += snprintf(output + n, rem, ", ");

        if (col->tem_agregacao) {
            n += snprintf(output + n, rem, "%s(%s)", col->agregacao, col->nome_coluna);
        } else {
            n += snprintf(output + n, rem, "%s", col->nome_coluna);
        }

        rem = maxlen - n;
        if (col->alias[0]) {
            n += snprintf(output + n, rem, " AS %s", col->alias);
        }
    }

    n += snprintf(output + n, maxlen - n, "\nFROM %s", ast->tabela);

    if (ast->tem_where) {
        n += snprintf(output + n, maxlen - n, "\nWHERE %s LIKE %s", ast->where_coluna, ast->where_valor);
    }

    if (ast->n_groupby) {
        n += snprintf(output + n, maxlen - n, "\nGROUP BY ");
        for (int i = 0; i < ast->n_groupby; i++) {
            if (i > 0) n += snprintf(output + n, maxlen - n, ", ");
            n += snprintf(output + n, maxlen - n, "%s", ast->groupby_cols[i]);
        }
    }

    if (ast->n_orderby) {
        n += snprintf(output + n, maxlen - n, "\nORDER BY ");
        for (int i = 0; i < ast->n_orderby; i++) {
            if (i > 0) n += snprintf(output + n, maxlen - n, ", ");
            n += snprintf(output + n, maxlen - n, "%s", ast->orderby_cols[i]);
            if (ast->orderby_descendente[i]) {
                n += snprintf(output + n, maxlen - n, " DESC");
            }
        }
    }

    n += snprintf(output + n, maxlen - n, ";");
}


#include "header.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

API const char* translate_to_sql(const char* custom_command) {
    static __thread char error_msg[128];

    Lexer lexer = { .src = custom_command, .pos = 0 };
    lexer_tokenize(&lexer);

    Parser parser = { .tokens = lexer.tokens, .pos = 0 };
    Token *t = parser_peek(&parser);

    char *sql = (char*)malloc(4096);
    if (!sql) return NULL;

    sql[0] = 0;

    if (t->type == TK_INSERE) {
        ASTInsert ast = {0};
        parse_insert(&parser, &ast);
        semantic_check(&ast);
        optimize(&ast);
        generate_insert_sql(&ast, sql, 4096);
    }
    else if (t->type == TK_CRIAR) {
        ASTCreateTable ast = {0};
        parse_create_table(&parser, &ast);
        generate_create_sql(&ast, sql, 4096);
    }
    else if (t->type == TK_SELECIONE) {
        ASTSelect ast = {0};
        parse_select(&parser, &ast);
        generate_select_sql(&ast, sql, 4096);
    }
    else {
        free(sql);
        return NULL;
    }

    return sql;
}

API void free_translated_string(const char* sql_string) {
    free((void*)sql_string);
}


int main() {
    char input[4096];

    printf("Digite o comando na linguagem personalizada:\nexemplo CREATE, INSERT ou SELECT\n> ");

    if (!fgets(input,sizeof(input),stdin)) {
        fprintf(stderr,"Erro ao ler entrada\n");
        return 1;
    }

    size_t len = strlen(input);
    if (len>0 && input[len-1] == '\n') input[len-1] = 0;

    Lexer lexer = { .src = input, .pos = 0 };
    lexer_tokenize(&lexer);

    Parser parser = { .tokens = lexer.tokens, .pos = 0 };
    Token *t = parser_peek(&parser);

    char sql[4096] = {0};

    if (t->type == TK_INSERE) {
        ASTInsert ast = {0};
        parse_insert(&parser, &ast);
        semantic_check(&ast);
        optimize(&ast);
        generate_insert_sql(&ast, sql, sizeof(sql));
    }
    else if (t->type == TK_CRIAR) {
        ASTCreateTable ast = {0};
        parse_create_table(&parser, &ast);
        generate_create_sql(&ast, sql, sizeof(sql));
    }
    else if (t->type == TK_SELECIONE) {
        ASTSelect ast = {0};
        parse_select(&parser, &ast);
        generate_select_sql(&ast, sql, sizeof(sql));
    }
    else {
        fprintf(stderr, "Comando desconhecido ou não suportado no começo: %s\n", t->text);
        return 1;
    }

    printf("\nSQL gerado:\n%s\n", sql);

    return 0;
}