{
  "targets": [
    {
      "target_name": "hello",
      "sources": [ "src/main.cc", "src/layout.cc", "src/quadTree.cc" ],
      "cflags": [
       "-fopenmp", "-std=c++11", "-fpermissive", "-fexceptions"
      ],
      'cflags!': [ '-fno-exceptions' ],
      'cflags_cc!': [ '-fno-exceptions' ]
    }
  ]
}
