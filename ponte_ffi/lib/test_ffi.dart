import 'dart:ffi';
import 'package:ffi/ffi.dart'; // para toNativeUtf8 e toDartString

void main() {
  // Use o nome correto da sua biblioteca:
  // final dylib = DynamicLibrary.open('libinterpreter.so'); // Linux
  final dylib = DynamicLibrary.open('libinterpreter.dylib'); // macOS
  // final dylib = DynamicLibrary.open('interpreter.dll'); // Windows

  final translateToSql = dylib.lookupFunction<
      Pointer<Utf8> Function(Pointer<Utf8>),
      Pointer<Utf8> Function(Pointer<Utf8>)>('translate_to_sql');

  final freeTranslatedString = dylib.lookupFunction<
      Void Function(Pointer<Utf8>),
      void Function(Pointer<Utf8>)>('free_translated_string');

  const command = 'SELECIONE nome DE alunos;';
  final commandPtr = command.toNativeUtf8();
  final sqlPtr = translateToSql(commandPtr);
  if (sqlPtr.address != 0) {
    print(sqlPtr.toDartString());
    freeTranslatedString(sqlPtr);
  } else {
    print('Erro ao traduzir comando');
  }
  calloc.free(commandPtr); // Use calloc/free se usar o package:ffi/ffi.dart
}
