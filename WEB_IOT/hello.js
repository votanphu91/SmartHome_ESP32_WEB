var den1 = "OFF";
var den2 = "OFF";
var den3 = "OFF";
var den4 = "OFF";
var den5 = "OFF";
var quat = "OFF";
var trom = " OFF";
var state_d1 = false;
var state_d2 = false;
var state_d3 = false;
var state_d4 = false;
var state_d5 = false;
var state_q = false;
var state_trom = false;
var nhietdodieuhoa = 27;
var trunggian;
var trangthaidieuhoa;
window.onload = function() {
  writedatatofirebase(den1, den2, den3, den4, den5, quat);
}
function changeImage1() {
  var lightBulb = document.getElementById("light_image1");
  
  if (lightBulb.src.endsWith("image/light_off.png")) {
    lightBulb.src = "image/light_on.png";
  } else {
    lightBulb.src = "image/light_off.png";
  }
  state_d1=!state_d1;
  if(state_d1){den1 = "ON";}
  else{
    den1 = "OFF";
  }
  writedatatofirebase(den1,den2,den3,den4,den5,quat);
}
function changeImage2() {
  var lightBulb = document.getElementById("light_image2");
  
  if (lightBulb.src.endsWith("image/light_off.png")) {
    lightBulb.src = "image/light_on.png";
  } else {
    lightBulb.src = "image/light_off.png";
  }
  state_d2=!state_d2;
  if(state_d2){den2 = "ON";}
  else{
    den2 = "OFF";
  }
  writedatatofirebase(den1,den2,den3,den4,den5,quat);
}
function changeImage3() {
  var lightBulb = document.getElementById("light_image3");
  
  if (lightBulb.src.endsWith("image/light_off.png")) {
    lightBulb.src = "image/light_on.png";
  } else {
    lightBulb.src = "image/light_off.png";
  }
  state_d3=!state_d3;
  if(state_d3){den3 = "ON";}
  else{
    den3 = "OFF";
  }
  writedatatofirebase(den1,den2,den3,den4,den5,quat);
}
function changeImage4() {
  var lightBulb = document.getElementById("light_image4");
  
  if (lightBulb.src.endsWith("image/light_off.png")) {
    lightBulb.src = "image/light_on.png";
  } else {
    lightBulb.src = "image/light_off.png";
  }
  state_d4=!state_d4;
  if(state_d4){den4 = "ON";}
  else{
    den4 = "OFF";
  }
  writedatatofirebase(den1,den2,den3,den4,den5,quat);
}
function changeImage5() {
  var lightBulb = document.getElementById("light_image5");
  
  if (lightBulb.src.endsWith("image/light_off.png")) {
    lightBulb.src = "image/light_on.png";
  } else {
    lightBulb.src = "image/light_off.png";
  }
  state_d5=!state_d5;
  if(state_d5){den5 = "ON";}
  else{
    den5 = "OFF";
  }
  writedatatofirebase(den1,den2,den3,den4,den5,quat);
}
function changeImage6() {
  var lightBulb = document.getElementById("quat");
  
  if (lightBulb.src.endsWith("image/qq_off.png")) {
    lightBulb.src = "image/qq_on.png";
  } else {
    lightBulb.src = "image/qq_off.png";
  }
  state_q=!state_q;
  if(state_q){quat = "ON";}
  else{
    quat = "OFF";
  }
  writedatatofirebase(den1,den2,den3,den4,den5,quat);
}
function changeImage7() {
  var lightBulb = document.getElementById("dieuhoa");
  
  if (lightBulb.src.endsWith("image/dieuhoa_off.png")) {
    lightBulb.src = "image/dieuhoa_on.png";
    firebase.database().ref("trangthai").set({dh:"ON"})
  } else {
    lightBulb.src = "image/dieuhoa_off.png";
    firebase.database().ref("trangthai").set({dh:"OFF"})
  }
  trunggian = nhietdodieuhoa;
  
}

function writedatatofirebase(a,b,c,d,e,f){
  firebase.database().ref("thietbi").set({
    den1:a,
    den2:b,
    den3:c,
    den4:d,
    den5:e,
    quat:f
  })
}
function writedatatofirebase1(g){
  firebase.database().ref("sc").set({
    trom:g
  })
}
// security------------//
function changeImage8() {
  var lightBulb = document.getElementById("image_robber");
  
  if (lightBulb.src.endsWith("image/alarmOFF.png")) {
    lightBulb.src = "image/alarmON.png";
  } else {
    lightBulb.src = "image/alarmOFF.png";
  }
  state_trom=!state_trom;
  if(state_trom){trom = "ON";}
  else{
    trom = "OFF";
  }
  writedatatofirebase1(trom);
}
function cong(){
  trunggian+=1;
  firebase.database().ref("dieuhoa").set({nhietdo:trunggian})
}
function tru(){
  trunggian-=1;
  firebase.database().ref("dieuhoa").set({nhietdo:trunggian})
}
 