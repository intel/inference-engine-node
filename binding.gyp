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
        './src/ie_backend.cc',
        './src/ie_binding.cc'
      ],
      'cflags_cc!': [ '-fno-rtti' ],
      'cflags_cc': [ '-fexceptions' ],
      'include_dirs' : [ '<(IE_INCLUDE_DIR)' ],
      'library_dirs' : ['<(IE_LIBRARY_DIR)'],
      'libraries' : [
        '-linference_engine'
      ]
    }
  ]
}