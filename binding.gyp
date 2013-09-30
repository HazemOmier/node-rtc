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
      'link_settings': {
        'libraries': [
          '../third_party/trunk/out/Debug/libCNG.a',
          '../third_party/trunk/out/Debug/libG711.a',
          '../third_party/trunk/out/Debug/libG722.a',
          '../third_party/trunk/out/Debug/libNetEq.a',
          '../third_party/trunk/out/Debug/libNetEq4.a',
          '../third_party/trunk/out/Debug/libPCM16B.a',
          '../third_party/trunk/out/Debug/libacm2.a',
          '../third_party/trunk/out/Debug/libaudio_coding_module.a',
          '../third_party/trunk/out/Debug/libaudio_conference_mixer.a',
          '../third_party/trunk/out/Debug/libaudio_device.a',
          '../third_party/trunk/out/Debug/libaudio_processing.a',
          '../third_party/trunk/out/Debug/libaudio_processing_sse2.a',
          '../third_party/trunk/out/Debug/libaudioproc_debug_proto.a',
          '../third_party/trunk/out/Debug/libbitrate_controller.a',
          '../third_party/trunk/out/Debug/libcommon_audio.a',
          '../third_party/trunk/out/Debug/libcommon_audio_sse2.a',
          '../third_party/trunk/out/Debug/libcommon_video.a',
          '../third_party/trunk/out/Debug/libdesktop_capture.a',
          '../third_party/trunk/out/Debug/libdesktop_capture_differ_sse2.a',
          '../third_party/trunk/out/Debug/libiLBC.a',
          '../third_party/trunk/out/Debug/libiSAC.a',
          '../third_party/trunk/out/Debug/libiSACFix.a',
          '../third_party/trunk/out/Debug/libjingle.a',
          '../third_party/trunk/out/Debug/libjingle_media.a',
          '../third_party/trunk/out/Debug/libjingle_p2p.a',
          '../third_party/trunk/out/Debug/libjingle_peerconnection.a',
          '../third_party/trunk/out/Debug/libmedia_file.a',
          '../third_party/trunk/out/Debug/libpaced_sender.a',
          '../third_party/trunk/out/Debug/libremote_bitrate_estimator.a',
          '../third_party/trunk/out/Debug/librtp_rtcp.a',
          '../third_party/trunk/out/Debug/libsystem_wrappers.a',
          '../third_party/trunk/out/Debug/libvideo_capture_module.a',
          '../third_party/trunk/out/Debug/libvideo_engine_core.a',
          '../third_party/trunk/out/Debug/libvideo_processing.a',
          '../third_party/trunk/out/Debug/libvideo_processing_sse2.a',
          '../third_party/trunk/out/Debug/libvideo_render_module.a',
          '../third_party/trunk/out/Debug/libvoice_engine.a',
          '../third_party/trunk/out/Debug/libwebrtc_i420.a',
          '../third_party/trunk/out/Debug/libwebrtc_opus.a',
          '../third_party/trunk/out/Debug/libwebrtc_utility.a',
          '../third_party/trunk/out/Debug/libwebrtc_video_coding.a',
          '../third_party/trunk/out/Debug/libwebrtc_vp8.a',
          '../third_party/trunk/out/Debug/libprotobuf_lite.a',
          '../third_party/trunk/out/Debug/librbe_components.a',
          '../third_party/trunk/out/Debug/libopus.a',
          '../third_party/trunk/out/Debug/libsrtp.a'
        ]
      },
      'defines': [
        'POSIX'
      ],
      'conditions': [
        [ "OS == 'mac'", {
            'libraries': [
              '$(SDKROOT)/System/Library/Frameworks/QTKit.framework',
            ]
          }
        ]
      ]
    }
  ]
}
