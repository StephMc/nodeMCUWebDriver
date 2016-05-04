function loadDoc() {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
      if (xhttp.readyState == 4 && xhttp.status == 200) {
           document.getElementById("test").innerHTML = xhttp.responseText;
             }
      };
  xhttp.open("POST", "x="+joystick.deltaX()+"&y="+joystick.deltaY(), true);
  xhttp.send("x="+joystick.deltaX()+"&y="+joystick.deltaY());
  return joystick.deltaX();
}  
