// interpreter.h
#ifdef _WIN32
#define API __declspec(dllexport)
#else
#define API
#endif

#ifdef __cplusplus
extern "C" {
#endif

/// Transforma comando customizado em SQL.
/// Retorna NULL em caso de erro sintático.
/// O ponteiro retornado deve ser liberado com free_translated_string.
API const char* translate_to_sql(const char* custom_command);

/// Libera a memória da string SQL devolvida pela função acima.
API void free_translated_string(const char* sql_string);

#ifdef __cplusplus
}
#endif