/***
 *             __         _
 *      _   _ / _| ___   (_)___
 *     | | | | |_ / _ \  | / __|
 *     | |_| |  _| (_) | | \__
 *      \__,_|_|  \___(_)/ |___/
 *                     |__/
 *
 *     Proudly developed by:
 *
 *     boemianrapsodi   @gmail.com
 *     b3by.in.th3.sky  @gmail.com
 *
 *
 *     GitHub URL:
 *
 *     https://github.com/organizations/ufojs
 *
 *   This project is completely open source, and can be
 *   found on GitHub. So, if you like it and would like
 *   to collaborate, just let us know. We're totally on
 *   this sh*t, and appreciate comments and suggestions
 *   as much as we do for bacon.
 *   Do you know what else do we like? Bacon.
 *
 */

#ifndef NODE_RTC_PEERCONNECTION_WRAPPER_H
#define NODE_RTC_PEERCONNECTION_WRAPPER_H

#include <node.h>
#include "../third_party/trunk/talk/app/webrtc/peerconnectioninterface.h"

class PeerconnectionWrapper :
  public node::ObjectWrap {
    public:
      static void Init(v8::Handle<v8::Object> exports);

      PeerconnectionWrapper();
      ~PeerconnectionWrapper();

      static v8::Handle<v8::Value> New(const v8::Arguments& args);
      static v8::Handle<v8::Value> AddStream(const v8::Arguments& args);
      static v8::Handle<v8::Value> CreateOffer(const v8::Arguments& args);
      static v8::Handle<v8::Value> CreateAnswer(const v8::Arguments& args);
      static v8::Handle<v8::Value> SetLocalDescription(const v8::Arguments& args);
      static v8::Handle<v8::Value> SetRemoteDescription(const v8::Arguments& args);
      static v8::Handle<v8::Value> AddIceCandidate(const v8::Arguments& args);

    private:
      static void createSessionDescriptionCallbackHandler(uv_async_t *handle, int status);
      static void setSessionDescriptionCallbackHandler(uv_async_t *handle, int status);
      static void peerConnectionObserverCallbackHandler(uv_async_t *handle, int status);

      talk_base::scoped_refptr<webrtc::PeerConnectionInterface> peerConnection;
      talk_base::scoped_refptr<webrtc::PeerConnectionFactoryInterface> peerConnectionFactory;

      uv_async_t createSessionDescriptionAsyncHandler;
      uv_async_t setSessionDescriptionAsyncHandler;
      uv_async_t peerConnectionObserverAsyncHandler;

      struct CreateSessionDescriptionObserverData {
        v8::Persistent<v8::Value> onSuccessCallback;
        v8::Persistent<v8::Value> onFailureCallback;
        webrtc::SessionDescriptionInterface* description;
        std::string error;
      };

      struct SetSessionDescriptionObserverData {
        v8::Persistent<v8::Value> onSuccessCallback;
        v8::Persistent<v8::Value> onFailureCallback;
      };

      struct PeerConnectionObserverData {
        v8::Persistent<v8::Object> thisPointer;
        std::string* candidate;
        std::string event;
      };

    public:
      class CreateSessionDescriptionObserverImpl : public webrtc::CreateSessionDescriptionObserver {
        public:
          CreateSessionDescriptionObserverImpl(PeerconnectionWrapper* observerClass) { this->wrapperObserver = observerClass; }
          virtual void OnSuccess(webrtc::SessionDescriptionInterface* desc) {
            CreateSessionDescriptionObserverData* data = (CreateSessionDescriptionObserverData*) this->wrapperObserver->createSessionDescriptionAsyncHandler.data;
            data->description = desc;
            uv_async_send(&this->wrapperObserver->createSessionDescriptionAsyncHandler);
          }
          virtual void OnFailure(const std::string& error) { 
            CreateSessionDescriptionObserverData* data = (CreateSessionDescriptionObserverData*) this->wrapperObserver->createSessionDescriptionAsyncHandler.data;
            data->error = error;
            uv_async_send(&this->wrapperObserver->createSessionDescriptionAsyncHandler); 
          }
          ~CreateSessionDescriptionObserverImpl() { puts("CreateSessionDescriptionObserverImpl destroyed"); }
        private:
          PeerconnectionWrapper* wrapperObserver;
      };

      class PeerConnectionObserverImpl : public webrtc::PeerConnectionObserver {
        public:
          PeerConnectionObserverImpl(PeerconnectionWrapper* observerClass) { this->wrapperObserver = observerClass; }
          virtual void OnError() {}
          virtual void OnStateChange(webrtc::PeerConnectionObserver::StateType state_changed) {}
          virtual void OnAddStream(webrtc::MediaStreamInterface* stream) {}
          virtual void OnRemoveStream(webrtc::MediaStreamInterface* stream) {}
          virtual void OnRenegotiationNeeded() {}
          virtual void OnIceChange() {}
          virtual void OnIceCandidate(const webrtc::IceCandidateInterface* candidate) {
            PeerConnectionObserverData* data = (PeerConnectionObserverData*) this->wrapperObserver->peerConnectionObserverAsyncHandler.data;
            data->event = "onIceCandidate";

            std::string* candidateAsString = new std::string();
            candidate->ToString(candidateAsString);
            data->candidate = candidateAsString;

            uv_async_send(&this->wrapperObserver->peerConnectionObserverAsyncHandler);
          }
          ~PeerConnectionObserverImpl() { puts("PeerConnectionObserverImpl destroyed"); }
        private:
          PeerconnectionWrapper* wrapperObserver;
      };

      class SetSessionDescriptionObserverImpl : public webrtc::SetSessionDescriptionObserver {
        public:
          SetSessionDescriptionObserverImpl(PeerconnectionWrapper* observerClass) { this->wrapperObserver = observerClass; }
          virtual void OnSuccess() {
            uv_async_send(&this->wrapperObserver->setSessionDescriptionAsyncHandler);
          }
          virtual void OnFailure(const std::string& error) {}
          SetSessionDescriptionObserverImpl() { puts("SetSessionDescriptionObserverImpl destroyed"); }
        private:
          PeerconnectionWrapper* wrapperObserver;
      };
  };

#endif

