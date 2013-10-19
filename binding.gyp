{
  'targets': [
    {
      'target_name': 'rtc',
      'sources': [
        'src/rtc.cc',
        'src/peerconnection_wrapper.cc'
      ],
      'include_dirs': [
        './third_party/trunk',
      ],
      'defines': [
        'POSIX'
      ],
      'link_settings': {
        'libraries': [
          '../third_party/trunk/xcodebuild/Debug/libwebrtc.a'
        ]
      },
      'conditions': [
        [ "OS == 'mac'", {
            'libraries': [
              '$(SDKROOT)/System/Library/Frameworks/QTKit.framework'
            ]
          }
        ]
      ]
    }
  ]
}
