var ws = new WebSocket('ws://localhost:8080');

describe('Browser side behavior', function() {
  it('Should complete handshake', function(done) {
    ws.onmessage = function(event) {
      console.log(event);
    };

    var onOffer = function(offer) {
      var msg = { type: 'offer', body: offer };
      //ws.send(msg);
      done();
    };

    var pc = new webkitRTCPeerConnection({ iceServers: [ { url : "stun:stun.l.google.com:19302" } ] }, { optional: [ { RtpDataChannels: true } ] });
    pc.createOffer(onOffer);
  });
});

