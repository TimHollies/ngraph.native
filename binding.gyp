{
  "targets": [
    {
      "target_name": "force-layout",
      "sources": [ "src/main.cc", "src/layout.cc", "src/quadTree.cc" ],
      "cflags": [
      "-std=c++11", "-fpermissive", "-fexceptions"
      ],
      'cflags!': [ '-fno-exceptions' ],
      'cflags_cc!': [ '-fno-exceptions' ]
    }
  ]
}
