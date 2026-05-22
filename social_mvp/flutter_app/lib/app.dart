import 'package:flutter/material.dart';

class CommunitySocialApp extends StatelessWidget {
  const CommunitySocialApp({super.key});

  @override
  Widget build(BuildContext context) {
    return MaterialApp(
      title: 'Community Social MVP',
      theme: ThemeData(useMaterial3: true, colorSchemeSeed: Colors.indigo),
      home: const Scaffold(
        body: Center(
          child: Text('Community Social MVP scaffold ready.'),
        ),
      ),
    );
  }
}
