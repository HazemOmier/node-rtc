#include "peerconnection_wrapper.h"

using namespace v8;

/**
 *  The initialization method.
 *  This method is used to initialize the module. It sets the prototype of the
 *  current object.
 *
 *  @param exports The node exports variable.
 */
void PeerconnectionWrapper::Init(Handle<Object> exports) {

  Local<FunctionTemplate> tpl = FunctionTemplate::New(New);
  tpl->SetClassName(String::NewSymbol("NodeRTCPeerconnection"));

  tpl->PrototypeTemplate()->Set(String::NewSymbol("addStream"), FunctionTemplate::New(AddStream)->GetFunction());
  tpl->PrototypeTemplate()->Set(String::NewSymbol("createOffer"), FunctionTemplate::New(CreateOffer)->GetFunction());
  tpl->PrototypeTemplate()->Set(String::NewSymbol("createAnswer"), FunctionTemplate::New(CreateAnswer)->GetFunction());
  tpl->PrototypeTemplate()->Set(String::NewSymbol("setLocalDescription"), FunctionTemplate::New(SetLocalDescription)->GetFunction());
  tpl->PrototypeTemplate()->Set(String::NewSymbol("setRemoteDescription"), FunctionTemplate::New(SetRemoteDescription)->GetFunction());
  tpl->PrototypeTemplate()->Set(String::NewSymbol("addIceCandidate"), FunctionTemplate::New(AddIceCandidate)->GetFunction());

  tpl->InstanceTemplate()->SetInternalFieldCount(1);

  exports->Set(String::NewSymbol("NodeRTCPeerconnection"), tpl->GetFunction());

}

/**
 *  The javascript constructor.
 *  This is the constructor of the javascript class.
 *
 *  @return The handler of the wrapper.
 */
Handle<Value> PeerconnectionWrapper::New(const Arguments& args) {
  HandleScope scope;

  PeerconnectionWrapper* thisWrapper = new PeerconnectionWrapper();

  uv_async_init(uv_default_loop(), &thisWrapper->peerConnectionObserverAsyncHandler, PeerconnectionWrapper::peerConnectionObserverCallbackHandler);

  PeerConnectionObserverData* dataToPass = new PeerConnectionObserverData();
  dataToPass->thisPointer = Persistent<Object>::New(args.This());
  thisWrapper->peerConnectionObserverAsyncHandler.data = dataToPass;

  thisWrapper->Wrap(args.This());

  return scope.Close(args.This());
}

/**
 * AddIceCandidate method.
 * This methos add an ice candidate to PeerConnection.
 *
 * @param candidateObject is the current candidate as json object.
 */
Handle<v8::Value> PeerconnectionWrapper::AddIceCandidate(const Arguments& args) {
  HandleScope scope;

  if(args.Length() == 0)
    return ThrowException(Exception::Error(String::New("Not enough arguments")));
  if(!args[0]->IsObject())
    return ThrowException(Exception::Error(String::New("The type of an object was incompatible with the expected type of the parameter associated to the object.")));

  PeerconnectionWrapper* thisWrapper = ObjectWrap::Unwrap<PeerconnectionWrapper>(args.This());

  Handle<Object> candidateObject = Handle<Object>::Cast(args[0]);

  String::AsciiValue candidateASCII(candidateObject->Get(String::New("candidate"))->ToString());
  std::string candidate = std::string(*candidateASCII);

  String::AsciiValue sdpMidASCII(candidateObject->Get(String::New("sdpMid"))->ToString());
  std::string sdpMid = std::string(*sdpMidASCII);

  int32_t sdpMLineIndex = candidateObject->Get(String::New("sdpMid"))->Int32Value();

  webrtc::IceCandidateInterface* candidateToAdd = webrtc::CreateIceCandidate(sdpMid, sdpMLineIndex, candidate);
  thisWrapper->peerConnection->AddIceCandidate(candidateToAdd);

  return scope.Close(Undefined());
}

/**
 * AddStream method.
 * This methos add a local stream to PeerConnection.
 * ALERT Right now this is bypassed. It creates the stream locally
 */
Handle<Value> PeerconnectionWrapper::AddStream(const Arguments& args) {
  HandleScope scope;

  PeerconnectionWrapper* thisWrapper = ObjectWrap::Unwrap<PeerconnectionWrapper>(args.This());

  talk_base::scoped_refptr<webrtc::MediaStreamInterface> stream = thisWrapper->peerConnectionFactory->CreateLocalMediaStream("dumb stream");
  thisWrapper->peerConnection->AddStream(stream, NULL);

  return scope.Close(Undefined());
}

/**
 * CreateOffer method.
 * With this method you can create a SDP offer to send to callee.
 *
 * @param onSuccessCallback is called if offer is created successfully.
 * @param onFailureCallback is called if something goes wrong.
 */
Handle<Value> PeerconnectionWrapper::CreateOffer(const Arguments& args) {
  HandleScope scope;

  if(args.Length() == 0)
    return ThrowException(Exception::Error(String::New("Not enough arguments")));

  PeerconnectionWrapper* thisWrapper = ObjectWrap::Unwrap<PeerconnectionWrapper>(args.This());

  CreateSessionDescriptionObserverData* dataToPass = new CreateSessionDescriptionObserverData();
  dataToPass->onSuccessCallback = Persistent<Value>::New(args[0]);
  dataToPass->onFailureCallback = Persistent<Value>::New(args[1]);
  thisWrapper->createSessionDescriptionAsyncHandler.data = (void*) dataToPass;

  uv_async_init(uv_default_loop(), &thisWrapper->createSessionDescriptionAsyncHandler, PeerconnectionWrapper::createSessionDescriptionCallbackHandler);

  talk_base::scoped_refptr<CreateSessionDescriptionObserverImpl> obs = new talk_base::RefCountedObject<CreateSessionDescriptionObserverImpl>(thisWrapper);
  thisWrapper->peerConnection->CreateOffer(obs, NULL);

  return scope.Close(Undefined());
}

/**
 * CreateAnswer method.
 * With this method you can create a SDP answer to send to callee.
 *
 * @param onSuccessCallback is called if offer is created successfully.
 * @param onFailureCallback is called if something goes wrong.
 */
Handle<Value> PeerconnectionWrapper::CreateAnswer(const Arguments& args) {
  HandleScope scope;

  if(args.Length() == 0)
    return ThrowException(Exception::Error(String::New("Not enough arguments")));

  PeerconnectionWrapper* thisWrapper = ObjectWrap::Unwrap<PeerconnectionWrapper>(args.This());

  CreateSessionDescriptionObserverData* dataToPass = new CreateSessionDescriptionObserverData();
  dataToPass->onSuccessCallback = Persistent<Value>::New(args[0]);
  dataToPass->onFailureCallback = Persistent<Value>::New(args[1]);
  thisWrapper->createSessionDescriptionAsyncHandler.data = (void*) dataToPass;

  uv_async_init(uv_default_loop(), &thisWrapper->createSessionDescriptionAsyncHandler, PeerconnectionWrapper::createSessionDescriptionCallbackHandler);

  talk_base::scoped_refptr<CreateSessionDescriptionObserverImpl> obs = new talk_base::RefCountedObject<CreateSessionDescriptionObserverImpl>(thisWrapper);
  thisWrapper->peerConnection->CreateAnswer(obs, NULL);

  return scope.Close(Undefined());
}

/**
 * SetLocalDescription method.
 * This method is used to set a locally created session description to the peerconnection.
 */
Handle<Value> PeerconnectionWrapper::SetLocalDescription(const Arguments& args) {
  HandleScope scope;

  if(args.Length() == 0)
    return ThrowException(Exception::Error(String::New("Not enough arguments")));
  if(!args[0]->IsObject())
    return ThrowException(Exception::Error(String::New("The type of an object was incompatible with the expected type of the parameter associated to the object.")));

  PeerconnectionWrapper* thisWrapper = ObjectWrap::Unwrap<PeerconnectionWrapper>(args.This());

  SetSessionDescriptionObserverData* dataToPass = new SetSessionDescriptionObserverData();
  dataToPass->onSuccessCallback = Persistent<Value>::New(args[1]);
  dataToPass->onFailureCallback = Persistent<Value>::New(args[2]);
  thisWrapper->setSessionDescriptionAsyncHandler.data = (void*) dataToPass;

  uv_async_init(uv_default_loop(), &thisWrapper->setSessionDescriptionAsyncHandler, PeerconnectionWrapper::setSessionDescriptionCallbackHandler);

  Handle<Object> descAsObject = Handle<Object>::Cast(args[0]);

  String::AsciiValue typeASCII(descAsObject->Get(String::New("type"))->ToString());
  std::string type = std::string(*typeASCII);

  String::AsciiValue sdpASCII(descAsObject->Get(String::New("sdp"))->ToString());
  std::string sdp = std::string(*sdpASCII);

  webrtc::SessionDescriptionInterface* description(webrtc::CreateSessionDescription(type, sdp));

  talk_base::scoped_refptr<SetSessionDescriptionObserverImpl> obs = new talk_base::RefCountedObject<SetSessionDescriptionObserverImpl>(thisWrapper);
  thisWrapper->peerConnection->SetLocalDescription(obs, description);

  return scope.Close(Undefined());
}

/**
 * SetRemoteDescription method.
 * This method is used to set a remotely created session description to the peerconnection.
 */
Handle<Value> PeerconnectionWrapper::SetRemoteDescription(const Arguments& args) {
  HandleScope scope;

  if(args.Length() == 0)
    return ThrowException(Exception::Error(String::New("Not enough arguments")));
  if(!args[0]->IsObject())
    return ThrowException(Exception::Error(String::New("The type of an object was incompatible with the expected type of the parameter associated to the object.")));

  PeerconnectionWrapper* thisWrapper = ObjectWrap::Unwrap<PeerconnectionWrapper>(args.This());

  SetSessionDescriptionObserverData* dataToPass = new SetSessionDescriptionObserverData();
  dataToPass->onSuccessCallback = Persistent<Value>::New(args[1]);
  dataToPass->onFailureCallback = Persistent<Value>::New(args[2]);
  thisWrapper->setSessionDescriptionAsyncHandler.data = (void*) dataToPass;

  uv_async_init(uv_default_loop(), &thisWrapper->setSessionDescriptionAsyncHandler, PeerconnectionWrapper::setSessionDescriptionCallbackHandler);

  Handle<Object> descAsObject = Handle<Object>::Cast(args[0]);

  String::AsciiValue typeASCII(descAsObject->Get(String::New("type"))->ToString());
  std::string type = std::string(*typeASCII);

  String::AsciiValue sdpASCII(descAsObject->Get(String::New("sdp"))->ToString());
  std::string sdp = std::string(*sdpASCII);

  webrtc::SessionDescriptionInterface* description(webrtc::CreateSessionDescription(type, sdp));

  talk_base::scoped_refptr<SetSessionDescriptionObserverImpl> obs = new talk_base::RefCountedObject<SetSessionDescriptionObserverImpl>(thisWrapper);
  thisWrapper->peerConnection->SetRemoteDescription(obs, description);

  return scope.Close(Undefined());
}

/**
 * The constructor.
 * With this a new wrapper will be created.
 */
PeerconnectionWrapper::PeerconnectionWrapper() {
  // Creating the factory will generates the PeerConnection object
  this->peerConnectionFactory = webrtc::CreatePeerConnectionFactory();

  // Hardcoded ice servers. They will be passed from javascript code
  webrtc::PeerConnectionInterface::IceServers servers;
  webrtc::PeerConnectionInterface::IceServer server;
  server.uri = "stun:stun.l.google.com:19302";
  servers.push_back(server);

  // Creating observer
  PeerConnectionObserverImpl* pcobs = new PeerConnectionObserverImpl(this);
  // Creating PeerConnection
  this->peerConnection = peerConnectionFactory->CreatePeerConnection(servers, NULL, NULL, pcobs);
}

/**
 * The distructor.
 * Here some cleanup procedures.
 */
PeerconnectionWrapper::~PeerconnectionWrapper() {
  puts("PeerconnectionWrapper destructed");
}

/**
 * Create session description callback handler.
 * This method was automatically called when a new sdp was created. 
 * It is used to fire the right javascript callback.
 */
void PeerconnectionWrapper::createSessionDescriptionCallbackHandler(uv_async_t *handle, int status) {
  CreateSessionDescriptionObserverData* data = (CreateSessionDescriptionObserverData*) handle->data;

  if(data->description && data->onSuccessCallback->IsFunction()) {
    Handle<Function> clbk = Handle<Function>::Cast(data->onSuccessCallback);

    Local<Object> jsDescription = Object::New();

    std::string* descAsString = new std::string();
    data->description->ToString(descAsString);

    jsDescription->SetPrototype(String::New("RTCSessionDescription"));
    jsDescription->Set(String::New("sdp"), String::New(descAsString->c_str()));
    jsDescription->Set(String::New("type"), String::New(data->description->type().c_str()));

    Local<Value> argv[1] = { jsDescription };
    clbk->Call(Context::GetCurrent()->Global(), 1, argv);
  }
  else if(data->onFailureCallback->IsFunction()) {
    Handle<Function> clbk = Handle<Function>::Cast(data->onFailureCallback);

    Local<Value> argv[1] = { String::New(data->error.c_str()) };
    clbk->Call(Context::GetCurrent()->Global(), 1, argv);
  }

  data->description = NULL;
  data->error = "";

}

/**
 * Set session description callback handler.
 * This method was automatically called when an sdp was setted.
 * It is used to fire the right javascript callback.
 */
void PeerconnectionWrapper::setSessionDescriptionCallbackHandler(uv_async_t *handle, int status) {
  SetSessionDescriptionObserverData* data = (SetSessionDescriptionObserverData*) handle->data;

  if(data->onSuccessCallback->IsFunction()) {
    Handle<Function> clbk = Handle<Function>::Cast(data->onSuccessCallback);
    Local<Value> argv[0] = { };
    clbk->Call(Context::GetCurrent()->Global(), 0, argv);
  }
}

/**
 * Peerconnection callback handler.
 * This method was automatically called when an ice candidate was created.
 * It is used to fire the right javascript callback.
 */
void PeerconnectionWrapper::peerConnectionObserverCallbackHandler(uv_async_t *handle, int status) {
  PeerConnectionObserverData* data = (PeerConnectionObserverData*) handle->data;

  Handle<Value> functionAsValue = data->thisPointer->Get(String::New(data->event.c_str()));
  if(functionAsValue->IsFunction()) {
    Handle<Function> clbk = Handle<Function>::Cast(functionAsValue);

    Local<Object> jsCandidate = Object::New();

    jsCandidate->SetPrototype(String::New("RTCIceCandidate"));
    jsCandidate->Set(String::New("candidate"), String::New(data->candidate->c_str()));
    jsCandidate->Set(String::New("sdpMLineIndex"), Integer::New(data->sdpMLineIndex));
    jsCandidate->Set(String::New("sdpMid"), String::New(data->sdpMid->c_str()));

    Local<Object> jsCandidateEvent = Object::New();
    jsCandidateEvent->SetPrototype(String::New("RTCIceCandidateEvent"));
    jsCandidateEvent->Set(String::New("candidate"), jsCandidate);

    Local<Value> argv[1] = { jsCandidateEvent };
    clbk->Call(Context::GetCurrent()->Global(), 1, argv);
  }
}
