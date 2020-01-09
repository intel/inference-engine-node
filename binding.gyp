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
        './src/binding.cc'
      ],
      'cflags!': [ '-fno-exceptions' ],
      'cflags_cc!': [ '-fno-exceptions', '-fno-rtti' ],
      'xcode_settings': {
        'GCC_ENABLE_CPP_EXCEPTIONS': 'YES',
        'CLANG_CXX_LIBRARY': 'libc++',
        'MACOSX_DEPLOYMENT_TARGET': '10.7',
      },
      'msvs_settings': {
        'VCCLCompilerTool': { 'ExceptionHandling': 1 },
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