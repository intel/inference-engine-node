{
  'variables':{
    'INTEL_OPENVINO_DIR' : '<!@(node -p \"process.env.INTEL_OPENVINO_DIR\")>',
  },
  'conditions': [
    ['OS=="linux"', {
      'variables' : {
        'IE_INCLUDE_DIR' : '$(INTEL_OPENVINO_DIR)/deployment_tools/inference_engine/include',
        'IE_LIBRARY_DIR' : '$(INTEL_OPENVINO_DIR)/deployment_tools/inference_engine/lib/intel64/'
      },
    }],
    ['OS=="win"', {
      'variables' : {
        'IE_INCLUDE_DIR' : '$(INTEL_OPENVINO_DIR)\\deployment_tools\\inference_engine\\include',
        'IE_LIBRARY_DIR' : '$(INTEL_OPENVINO_DIR)\\deployment_tools\\inference_engine\\lib\\intel64\\Release'
      },
    }],
  ],
  'targets': [
    {
      'target_name': 'inference_engine_node',
      'sources': [
        "<!@(node -p \"require('fs').readdirSync('./src').map(f=>'src/'+f).join(' ')\")"
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
              'RuntimeLibrary': 2 # MultiThreadedDLL (/MD)
            },
          },
        }
      },
      'include_dirs' : [
        '<!@(node -p "require(\'node-addon-api\').include")',
        '<(IE_INCLUDE_DIR)'
      ],
      'library_dirs' : [
        '<(IE_LIBRARY_DIR)'
      ],
      'libraries' : [
        '-linference_engine',
        '-linference_engine_legacy'
      ]
    }
  ]
}
