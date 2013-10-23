describe('Browser side behavior', function() {
  this.timeout(60000);
  it('Should complete handshake', function(done) {
    var ws = new WebSocket('ws://localhost:8080');
    var pc = new webkitRTCPeerConnection({ iceServers: [ { url : "stun:stun.l.google.com:19302" } ] }, { optional: [ { RtpDataChannels: true } ] });

    ws.onmessage = function(event) {
      var message = JSON.parse(event.data);
      console.log('Received: ' + message.type);
      if (message.type === 'answer') {
        pc.setRemoteDescription(new RTCSessionDescription(message.body), done, function(error) { logMsg(error); });
      }
      else {
        ws.send(JSON.stringify({ type: 'error', body: 'Wrong message received' }));
      }
    };

    ws.onopen = function() {
      var onOffer = function(offer) {
        pc.setLocalDescription(offer);
        var msg = JSON.stringify({ type: 'offer', body: offer });
        ws.send(msg);
      };

      pc.createOffer(onOffer);
    };
  });
});

