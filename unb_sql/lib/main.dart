import 'package:flutter/material.dart';
import 'package:flutter_bloc/flutter_bloc.dart';
import 'package:font_awesome_flutter/font_awesome_flutter.dart';

// APP
List<Color> cores = [
  const Color(0xFF201c1c),
];

void main() {
  // COMPILER
  // final dylib = ffi.DynamicLibrary.open('libadd.dylib');

  // final dylib = ffi.DynamicLibrary.open('other/libadd.dylib');
  // final add = dylib.lookupFunction<ffi.Int64 Function(ffi.Int64, ffi.Int64),
  //     int Function(int, int)>('add');
  // final res = add(20, 10);
  // print(res);

  // APP
  runApp(MultiBlocProvider(
    providers: [
      BlocProvider(
        create: (_) => ThemeCubit(),
      ),
      BlocProvider(
        create: (_) => MainPageCubit(),
      ),
    ],
    child: const MainApp(),
  ));
}

class MainApp extends StatelessWidget {
  const MainApp({super.key});

  @override
  Widget build(BuildContext context) {
    return BlocBuilder<ThemeCubit, ThemeData>(
      builder: (context, theme) => MaterialApp(
        debugShowCheckedModeBanner: false,
        theme: theme,
        home: LayoutBuilder(builder: (context, constraints) {
          if (constraints.maxWidth < 800) {
            return const Scaffold(
              body: Center(
                child: Text(
                  "Tamanho da tela nao é suportado, tente novamente com uma tela maior.",
                  style: TextStyle(color: Colors.white),
                ),
              ),
            );
          } else {
            return const Scaffold(
              body: Column(
                children: [
                  Header(),
                  Row(
                    children: [
                      LeftBar(),
                      Expanded(child: MiddleContent()),
                      RightBar(),
                    ],
                  ),
                ],
              ),
            );
          }
        }),
      ),
    );
  }
}

class ThemeCubit extends Cubit<ThemeData> {
  ThemeCubit() : super(_darkTheme);

  static final _lightTheme = ThemeData(
    useMaterial3: true,
    brightness: Brightness.light,
    colorScheme: const ColorScheme.light(
      primary: Colors.black,
      onPrimary: Colors.white,
      surface: Colors.white,
      onSurface: Colors.black,
      secondary: Colors.black12,
      tertiary: Colors.black26,
    ),
    textTheme: const TextTheme(
      displayLarge: TextStyle(
        fontSize: 72,
        fontWeight: FontWeight.bold,
        color: Colors.black,
      ),
    ),
    iconTheme: const IconThemeData(color: Colors.black),
  );

  static final _darkTheme = ThemeData(
    scrollbarTheme: ScrollbarThemeData(
      thumbColor: MaterialStateProperty.all(Colors.transparent),
    ),
    useMaterial3: true,
    brightness: Brightness.dark,
    colorScheme: const ColorScheme.dark(
      primary: Colors.white,
      onPrimary: Colors.black,
      surface: Colors.black,
      onSurface: Colors.white,
      secondary: Colors.white10,
      tertiary: Colors.white24,
    ),
    textTheme: const TextTheme(
      displayLarge: TextStyle(
        fontSize: 72,
        fontWeight: FontWeight.bold,
        color: Colors.white,
      ),
    ),
    iconTheme: const IconThemeData(color: Colors.white),
  );

  void toggleTheme() {
    emit(state.brightness == Brightness.dark ? _lightTheme : _darkTheme);
  }
}

Size getHeaderSize(context) {
  return Size(
    MediaQuery.of(context).size.width,
    MediaQuery.of(context).size.height / 20,
  );
}

class Header extends StatelessWidget {
  const Header({super.key});

  @override
  Widget build(BuildContext context) {
    Size headerSize = getHeaderSize(context);

    return Container(
      height: headerSize.height,
      width: headerSize.width,
      color: Theme.of(context).colorScheme.secondary,
      child: Row(
        children: [
          HeaderButton(
            onPressed: () {},
            iconData: FontAwesomeIcons.folder,
            title: "Arquivos",
          ),
          // HeaderButton(
          //   onPressed: () {},
          //   iconData: FontAwesomeIcons.penToSquare,
          //   title: "Editar",
          // ),
          HeaderButton(
            onPressed: () {},
            iconData: FontAwesomeIcons.wifi,
            title: "Conectar",
          ),
          HeaderButton(
            onPressed: () {
              context.read<MainPageCubit>().toggleTerminal();
            },
            iconData: FontAwesomeIcons.eye,
            title: "Visualização",
          ),
          HeaderButton(
            onPressed: () {
              context.read<MainPageCubit>().toggleRightBar();
            },
            iconData: FontAwesomeIcons.play,
            title: "Executar",
          ),
          HeaderButton(
            onPressed: () {
              context.read<MainPageCubit>().toggleRightBar();
            },
            iconData: FontAwesomeIcons.forwardStep,
            title: "Executar Tudo",
          ),
          HeaderButton(
            onPressed: () {
              context.read<MainPageCubit>().toggleRightBar();
            },
            iconData: FontAwesomeIcons.question,
            title: "Ajuda",
          ),
          const Spacer(),
          BlocBuilder<ThemeCubit, ThemeData>(
            builder: (context, theme) {
              return theme.brightness == Brightness.dark
                  ? IconButton(
                      onPressed: () {
                        BlocProvider.of<ThemeCubit>(context).toggleTheme();
                      },
                      icon: const Icon(
                        Icons.nightlight_round_sharp,
                      ),
                    )
                  : IconButton(
                      onPressed: () {
                        BlocProvider.of<ThemeCubit>(context).toggleTheme();
                      },
                      icon: const Icon(
                        Icons.wb_sunny_outlined,
                      ),
                    );
            },
          ),
        ],
      ),
    );
  }
}

class HeaderButton extends StatelessWidget {
  final Function()? onPressed;
  final IconData iconData;
  final String title;

  const HeaderButton({
    super.key,
    this.onPressed,
    required this.iconData,
    required this.title,
  });

  @override
  Widget build(BuildContext context) {
    return TextButton(
      onPressed: onPressed,
      style: TextButton.styleFrom(
        shape: const RoundedRectangleBorder(
          borderRadius: BorderRadius.zero, // Define bordas retas
        ),
      ),
      child: Row(
        children: [
          Padding(
            padding: const EdgeInsets.only(right: 4, top: 10, bottom: 10),
            child: Icon(
              iconData,
              size: 14,
            ),
          ),
          const SizedBox(width: 5),
          Text(title),
        ],
      ),
    );
  }
}

Size getLeftBarSize(context) {
  final headerUsedHeight = getHeaderSize(context).height;
  return Size(
    MediaQuery.of(context).size.width / 8,
    MediaQuery.of(context).size.height - headerUsedHeight,
  );
}

class LeftBar extends StatelessWidget {
  const LeftBar({super.key});

  @override
  Widget build(BuildContext context) {
    Size leftbarSize = getLeftBarSize(context);
    List<Widget> listServers = [
      const LeftBarItem(
        text: "DDL",
        icon: FontAwesomeIcons.database,
        iconColor: Color(0xfff1c632),
      ),
      const LeftBarItem(
        text: "DML",
        icon: FontAwesomeIcons.database,
        iconColor: Color(0xfff1c632),
      ),
      const LeftBarItem(
        text: "DQL",
        icon: FontAwesomeIcons.database,
        iconColor: Color(0xfff1c632),
      ),
      const LeftBarItem(
        text: "DCL",
        icon: FontAwesomeIcons.database,
        iconColor: Color(0xfff1c632),
      ),
    ];
    List<Widget> listDatabases = [
      const LeftBarItem(text: "Base de Dado 1", icon: Icons.storage_rounded),
      const LeftBarItem(text: "Base de Dado 2", icon: Icons.storage_rounded),
      const LeftBarItem(
        text: "Base de Dado 3",
        icon: Icons.storage_rounded,
      )
    ];
    List<Widget> listTables = [
      const LeftBarItem(text: "Tabela 1", icon: Icons.table_chart_outlined),
      const LeftBarItem(text: "Tabela 2", icon: Icons.table_chart_outlined),
      const LeftBarItem(text: "Tabela 3", icon: Icons.table_chart_outlined),
    ];

    return Material(
      color: Theme.of(context).colorScheme.secondary,
      child: Column(
        children: [
          SizedBox(
            width: leftbarSize.width,
            height: leftbarSize.height,
            child: ListView(
              children: [
                LeftBarTitle(
                  leftbarSize: leftbarSize,
                  title: "Arquivos",
                ),
                const SizedBox(height: 5),
                ...listServers
              ],
            ),
          ),
        ],
      ),
    );
  }
}

class LeftBarItem extends StatelessWidget {
  final String text;
  final IconData icon;
  final Color? iconColor;
  const LeftBarItem({
    super.key,
    required this.text,
    required this.icon,
    this.iconColor,
  });

  @override
  Widget build(BuildContext context) {
    return Padding(
      padding: const EdgeInsets.only(bottom: 5),
      child: Row(
        children: [
          SizedBox(width: MediaQuery.of(context).size.width * 0.014),
          Icon(
            icon,
            size: 14,
            color: iconColor,
          ),
          const SizedBox(width: 10),
          Flexible(
            child: Text(
              text,
              overflow: TextOverflow.visible,
              softWrap: true,
              style: const TextStyle(),
            ),
          ),
        ],
      ),
    );
  }
}

class LeftBarTitle extends StatelessWidget {
  final String title;

  const LeftBarTitle({
    super.key,
    required this.leftbarSize,
    required this.title,
  });

  final Size leftbarSize;

  @override
  Widget build(BuildContext context) {
    return TextButton(
      style: TextButton.styleFrom(
        shape: const RoundedRectangleBorder(
          borderRadius: BorderRadius.zero,
        ),
      ),
      onPressed: () {},
      child: SizedBox(
        width: leftbarSize.width,
        child: Padding(
          padding: const EdgeInsets.symmetric(horizontal: 10),
          child: Row(
            children: [
              const Icon(
                Icons.arrow_drop_down,
                size: 16,
              ),
              const SizedBox(
                width: 10,
              ),
              Text(
                title,
                style: const TextStyle(fontSize: 16),
                overflow: TextOverflow.visible,
                softWrap: true,
              ),
            ],
          ),
        ),
      ),
    );
  }
}

Size getRightBarSize(context) {
  final headerUsedHeight = getHeaderSize(context).height;

  final bool isRightBarVisible =
      BlocProvider.of<MainPageCubit>(context).state.isRightBarEnabled;
  if (isRightBarVisible) {
    return Size(
      MediaQuery.of(context).size.width / 8,
      MediaQuery.of(context).size.height - headerUsedHeight,
    );
  }
  return const Size(0, 0);
}

class RightBar extends StatelessWidget {
  const RightBar({super.key});

  @override
  Widget build(BuildContext context) {
    return BlocBuilder<MainPageCubit, MainPageState>(
      builder: (context, state) {
        Size rightBarSize = getRightBarSize(context);

        return state.isRightBarEnabled
            ? Material(
                color: Theme.of(context).colorScheme.secondary,
                child: Column(
                  children: [
                    SizedBox(
                      width: rightBarSize.width,
                      height: rightBarSize.height,
                    ),
                  ],
                ),
              )
            : const SizedBox();
      },
    );
  }
}

Size getTerminalSize(context) {
  final leftBarUsedWidth = getLeftBarSize(context).width;
  final rightBarUsedWidth = getRightBarSize(context).width;

  final bool isRightBarVisible =
      BlocProvider.of<MainPageCubit>(context).state.isRightBarEnabled;
  debugPrint("Right BAr enabled? $isRightBarVisible");

  if (isRightBarVisible) {
    return Size(
      MediaQuery.of(context).size.width - leftBarUsedWidth - rightBarUsedWidth,
      300,
    );
  }
  return Size(
    MediaQuery.of(context).size.width - leftBarUsedWidth,
    300,
  );
}

Size getCodeAreaSize(context) {
  final headerUsedHeight = getHeaderSize(context).height;
  final leftBarUsedWidth = getLeftBarSize(context).width;
  final rightBarUsedWidth = getRightBarSize(context).width;
  final terminalUsedHeight = getTerminalSize(context).height;

  final bool isTerminalVisible =
      BlocProvider.of<MainPageCubit>(context).state.isTerminalEnabled;

  final bool isRightBarVisible =
      BlocProvider.of<MainPageCubit>(context).state.isRightBarEnabled;

  if (isTerminalVisible) {
    return Size(
      MediaQuery.of(context).size.width - leftBarUsedWidth - rightBarUsedWidth,
      MediaQuery.of(context).size.height -
          headerUsedHeight -
          terminalUsedHeight,
    );
  } else if (isRightBarVisible) {
    debugPrint("Right BAr enabled");
    return Size(
      MediaQuery.of(context).size.width - leftBarUsedWidth - rightBarUsedWidth,
      MediaQuery.of(context).size.height - headerUsedHeight,
    );
  }
  debugPrint("Right BAr  NOT enabled");
  return Size(
    MediaQuery.of(context).size.width - leftBarUsedWidth,
    MediaQuery.of(context).size.height - headerUsedHeight,
  );
}

class MainPageState {
  final bool isTerminalEnabled;
  final bool isRightBarEnabled;
  final bool isLeftBarEnabled;

  MainPageState({
    this.isTerminalEnabled = false,
    this.isRightBarEnabled = false,
    this.isLeftBarEnabled = false,
  });

  MainPageState copyWith({
    bool? isTerminalEnabled,
    bool? isRightBarEnabled,
    bool? isLeftBarEnabled,
  }) {
    return MainPageState(
      isTerminalEnabled: isTerminalEnabled ?? this.isTerminalEnabled,
      isRightBarEnabled: isRightBarEnabled ?? this.isRightBarEnabled,
      isLeftBarEnabled: isLeftBarEnabled ?? this.isLeftBarEnabled,
    );
  }
}

class MainPageCubit extends Cubit<MainPageState> {
  MainPageCubit() : super(MainPageState());
  void toggleTerminal() {
    final res = state.isTerminalEnabled == true
        ? state.copyWith(isTerminalEnabled: false)
        : state.copyWith(isTerminalEnabled: true);
    emit(res);
  }

  void toggleRightBar() {
    final res = state.isRightBarEnabled == true
        ? state.copyWith(isRightBarEnabled: false)
        : state.copyWith(isRightBarEnabled: true);
    emit(res);
  }

  void toggleLeftBar() {
    final res = state.isLeftBarEnabled == true
        ? state.copyWith(isLeftBarEnabled: false)
        : state.copyWith(isLeftBarEnabled: true);
    emit(res);
  }
}

class MiddleContent extends StatelessWidget {
  const MiddleContent({super.key});

  @override
  Widget build(BuildContext context) {
    return BlocBuilder<MainPageCubit, MainPageState>(
      builder: (context, state) {
        return Column(
          children: [
            CodeArea(
              codeAreaSize: getCodeAreaSize(context),
            ),
            if (state.isTerminalEnabled) const TerminalArea(),
          ],
        );
      },
    );
  }
}

class CodeArea extends StatefulWidget {
  final Size codeAreaSize;
  const CodeArea({
    Key? key,
    required this.codeAreaSize,
  }) : super(key: key);

  @override
  _CodeAreaState createState() => _CodeAreaState();
}

class _CodeAreaState extends State<CodeArea> {
  final TextEditingController _controller = TextEditingController();
  final ScrollController _listViewScrollController = ScrollController();
  final ScrollController _textFieldScrollController = ScrollController();
  final FocusNode textFieldFocusNode = FocusNode();

  @override
  void initState() {
    super.initState();

    _textFieldScrollController.addListener(() {
      if (_listViewScrollController.hasClients) {
        _listViewScrollController.jumpTo(_textFieldScrollController.offset);
      }
    });
  }

  @override
  void dispose() {
    _controller.dispose();
    _textFieldScrollController.dispose();
    _listViewScrollController.dispose();
    textFieldFocusNode.dispose();
    super.dispose();
  }

  @override
  Widget build(BuildContext context) {
    return InkWell(
      onTap: () {
        textFieldFocusNode.requestFocus();
      },
      splashColor: Colors.transparent, // remove a "onda" ao clicar
      highlightColor: Colors.transparent, // remove o cinza ao segurar/clicar
      hoverColor: Colors.transparent, // remove o cinza ao passar o mouse
      focusColor: Colors.transparent,
      child: SizedBox(
        height: widget.codeAreaSize.height,
        width: widget.codeAreaSize.width,
        child: Row(
          children: [
            // Container(
            //   width: 30,
            //   color: Theme.of(context).colorScheme.tertiary,
            //   // not visible scrollbar
            //   child: ScrollConfiguration(
            //     behavior:
            //         ScrollConfiguration.of(context).copyWith(scrollbars: false),
            //     child: ListView.builder(
            //       itemCount: 100,
            //       controller: _listViewScrollController,
            //       itemBuilder: (context, index) {
            //         return Container(
            //           color: Colors.transparent,
            //           padding: index == 0
            //               ? const EdgeInsets.only(top: 5, right: 2)
            //               : const EdgeInsets.only(right: 2),
            //           child: Text(
            //             '${index + 1}',
            //             textAlign: TextAlign.right,
            //             style: TextStyle(
            //               color: Theme.of(context).colorScheme.onSurface,
            //               fontSize: 14.7,
            //             ),
            //           ),
            //         );
            //       },
            //     ),
            //   ),
            // ),
            // const SizedBox(width: 5),
            Expanded(
                child: Align(
              alignment: Alignment.topCenter,
              child: NumberedTextField(focusNode: textFieldFocusNode),
            )),
            // Expanded(
            //   child: TextField(
            //     onTap: () {
            //       // BlocProvider.of<CodeBloc>(context).add(
            //       //   ChangeSelectedLineEvent(
            //       //     getSelectedLine(_controller),
            //       //   ),
            //       // );
            //     },
            //     scrollController: _textFieldScrollController,
            //     controller: _controller,
            //     maxLines: null,
            //     expands: true,
            //     onChanged: (text) {
            //       // BlocProvider.of<CodeBloc>(context).add(
            //       //   ChangeSelectedLineEvent(
            //       //     getSelectedLine(_controller),
            //       //   ),
            //       // );
            //     },
            //     buildCounter: (
            //       BuildContext context, {
            //       required int currentLength,
            //       required bool isFocused,
            //       required int? maxLength,
            //     }) {
            //       return Container(
            //         color: Colors.transparent,
            //         // padding: index == 0
            //         //     ? const EdgeInsets.only(top: 5, right: 2)
            //         //     : const EdgeInsets.only(right: 2),
            //         child: Text(
            //           '${1}',
            //           textAlign: TextAlign.right,
            //           style: TextStyle(
            //             color: Theme.of(context).colorScheme.onSurface,
            //             fontSize: 14.7,
            //           ),
            //         ),
            //       );
            //     },
            //     decoration: InputDecoration(
            //       border: InputBorder.none,
            //       hintText: 'Escreva seu código aqui...',
            //       suffix: ValueListenableBuilder<TextEditingValue>(
            //         valueListenable: _controller,
            //         builder: (context, value, child) {
            //           int lineCount = '\n'.allMatches(value.text).length + 1;
            //           return Text('$lineCount lines');
            //         },
            //       ),
            //     ),
            //     style: const TextStyle(
            //       fontFamily: 'Courier',
            //       fontSize: 14.29,
            //     ),
            //   ),
            // ),
          ],
        ),
      ),
    );
  }
}

class TerminalArea extends StatefulWidget {
  const TerminalArea({
    super.key,
  });

  @override
  _TerminalAreaState createState() => _TerminalAreaState();
}

class _TerminalAreaState extends State<TerminalArea> {
  @override
  void initState() {
    super.initState();
  }

  @override
  Widget build(BuildContext context) {
    return BlocBuilder<MainPageCubit, MainPageState>(
      builder: (context, state) {
        final Size terminalSize = getTerminalSize(context);

        return Container(
          width: terminalSize.width,
          height: terminalSize.height,
          color: Theme.of(context).colorScheme.secondary,
          child: Container(),
        );
      },
    );
  }
}

class NumberedTextField extends StatefulWidget {
  final FocusNode focusNode;
  const NumberedTextField({super.key, required this.focusNode});

  @override
  _NumberedTextFieldState createState() => _NumberedTextFieldState();
}

class _NumberedTextFieldState extends State<NumberedTextField> {
  final TextEditingController _controller = TextEditingController();
  final ScrollController _scrollControllerText = ScrollController();
  final ScrollController _scrollControllerNumbers = ScrollController();

  int _lineCount = 1;

  @override
  void initState() {
    super.initState();

    // Atualiza contagem de linhas quando texto muda
    _controller.addListener(() {
      final lines = '\n'.allMatches(_controller.text).length + 1;
      if (lines != _lineCount) {
        setState(() {
          _lineCount = lines;
        });
      }
    });

    // Sincroniza scroll vertical entre número e textfield
    _scrollControllerText.addListener(() {
      if (_scrollControllerNumbers.offset != _scrollControllerText.offset) {
        _scrollControllerNumbers.jumpTo(_scrollControllerText.offset);
      }
    });
    _scrollControllerNumbers.addListener(() {
      if (_scrollControllerText.offset != _scrollControllerNumbers.offset) {
        _scrollControllerText.jumpTo(_scrollControllerNumbers.offset);
      }
    });
  }

  @override
  void dispose() {
    _controller.dispose();
    _scrollControllerText.dispose();
    _scrollControllerNumbers.dispose();
    super.dispose();
  }

  @override
  Widget build(BuildContext context) {
    return Row(
      crossAxisAlignment: _lineCount == 1
          ? CrossAxisAlignment.start
          : CrossAxisAlignment.center,
      children: [
        // Números das linhas
        Container(
          color: Theme.of(context).colorScheme.tertiary,
          padding: const EdgeInsets.symmetric(horizontal: 6, vertical: 8),
          child: SingleChildScrollView(
            physics: const NeverScrollableScrollPhysics(),
            controller: _scrollControllerNumbers,
            child: Column(
              crossAxisAlignment: CrossAxisAlignment.end,
              mainAxisSize: MainAxisSize.min,
              children: List.generate(
                _lineCount,
                (index) => Text(
                  '${index + 1}',
                  style: TextStyle(
                    color: Theme.of(context).colorScheme.surface,
                    fontSize: 14,
                    height: 1.5,
                    fontFamily: 'monospace',
                  ),
                ),
              ),
            ),
          ),
        ),

        // Input do texto
        Expanded(
          child: TextField(
            focusNode: widget.focusNode,
            controller: _controller,
            maxLines: null,
            style: const TextStyle(
              fontSize: 14,
              height: 1.5,
              fontFamily: 'monospace',
            ),
            scrollController: _scrollControllerText,
            decoration: const InputDecoration(
              border: InputBorder.none,
              contentPadding: EdgeInsets.symmetric(horizontal: 8, vertical: 8),
              fillColor: Colors.transparent,
              hoverColor: Colors.transparent,
              focusColor: Colors.transparent,
              enabledBorder: InputBorder.none,
              focusedBorder: InputBorder.none,
            ),
          ),
        ),
      ],
    );
  }
}
