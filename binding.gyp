{
  'conditions': [
    ['OS=="linux"', {
      'variables' : {
        'IE_INCLUDE_DIR' : '/opt/intel/openvino_2019.3.376/inference_engine/include/',
        'IE_LIBRARY_DIR' : '/opt/intel/openvino_2019.3.376/inference_engine/lib/intel64/'
      },
    }],
    ['OS=="win"', {
      'variables' : {
        'IE_INCLUDE_DIR' : 'C:\\Program Files (x86)\\IntelSWTools\\openvino\\inference_engine\\include',
        'IE_LIBRARY_DIR' : 'C:\\Program Files (x86)\\IntelSWTools\\openvino\\inference_engine\\lib\\intel64\\Release'
      },
    }],
  ],
  'targets': [
    {
      'target_name': 'ie_node',
      'sources': [
        './src/binding.cc',
        './src/network.cc',
        './src/network.h'
      ],
      'cflags!': [ '-fno-rtti' ],
      'cflags_cc!': [ '-fno-rtti' ],
      'cflags!': [ '-fno-exceptions' ],
      'cflags_cc!': [ '-fno-exceptions' ],
      'default_configuration': 'Release',
      'configurations': {
        'Debug': {
          'msvs_settings': {
            'VCCLCompilerTool': { 'ExceptionHandling': 1, 'RuntimeTypeInfo': 'true'},
          },
        },
        'Release': {
          'msvs_settings': {
            'VCCLCompilerTool': { 'ExceptionHandling': 1, 'RuntimeTypeInfo': 'true'},
          },
        }
      },
      'include_dirs' : [ "<!@(node -p \"require('node-addon-api').include\")",
                         '<(IE_INCLUDE_DIR)' ],
      'library_dirs' : ['<(IE_LIBRARY_DIR)'],
      'libraries' : [
        '-linference_engine'
      ]
    }
  ]
}