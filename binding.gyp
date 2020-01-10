{
  'conditions': [
    ['OS=="linux"', {
      'variables' : {
        'IE_INCLUDE_DIR' : '/opt/intel/computer_vision_sdk/inference_engine/include/',
        'IE_LIBRARY_DIR' : '/opt/intel/computer_vision_sdk/inference_engine/lib/ubuntu_16.04/intel64/'
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
        './src/core.cc',
        './src/core.h',
        './src/input_info.cc',
        './src/input_info.h',
        './src/network.cc',
        './src/network.h'
      ],
      'cflags!': [ '-fno-exceptions', '-fno-rtti'],
      'cflags_cc!': [ '-fno-exceptions', '-fno-rtti'],
      'default_configuration': 'Release',
      'configurations': {
        'Debug': {
          'msvs_settings': {
            'VCCLCompilerTool': {
              'ExceptionHandling': 1,
              'RuntimeTypeInfo': 'true',
              'RuntimeLibrary': 3 # MultiThreadedDebugDLL (/MDd)
            },
          },
        },
        'Release': {
          'msvs_settings': {
            'VCCLCompilerTool': { 
              'ExceptionHandling': 1,
              'RuntimeTypeInfo': 'true',
              'RuntimeLibrary': 1,    # MultiThreadedDebug (/MTd)
            },
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