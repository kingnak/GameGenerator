
###FUNCTIONS###


 // Kills cookie 
 function ggDeleteVar(name) {
     document.cookie = name + "=; expires=Thu, 01-Jan-70 00:00:01 GMT" + "; path=/";
 }
 // Creates cookie
 function ggSetVar(name, value, expires) {
     document.cookie = name + "=" + escape(value) + "; path=/" + ((expires == null) ? "" : "; expires=" + expires.toGMTString());
 }
 // Checks cookie
 function ggReadVar(name) {
     var fullName = name + "=";
     var val = 0;
     var strings = document.cookie.split(';');
     for (var i = 0; i < strings.length; i++) {
         var str = strings[i];
         while (str.charAt(0) == ' ') str = str.substring(1, str.length);
         if (str.indexOf(fullName) == 0) val = parseInt(str.substring(fullName.length, str.length));
         if (isNaN(val)) val = 0;
     }
     return val;
 }
 // Alters cookie value 
 function ggVarPlus(name, amount) {
     var val = ggReadVar(name);
     val += amount;
     ggSetVar(name, val);
 }
 // Alters cookie value lower 
 function ggVarMinus(name, amount) {
     var val = ggReadVar(name);
     val -= amount;
     ggSetVar(name, val);
 }
 // Other disable back 
 function ggDisallowNav() {
     var disallowNav = 1;
     if (disallowNav) {
         document.oncontextmenu = blockContextMenu;
         document.onkeydown = blockKeyPresses;
         // This part works for IE, but not Firefox         
         undoBack();
         // This extra part is need for Firefox     
         window.onload = window.onload;
         undoBack();
         window.onpageshow = function(evt) {
             if (evt.persisted) undoBack()
         };
         window.onunload = function() {
             void(0)
         };
     }
 }
 // No back button 
 function undoBack() {
     window.history.forward();
 }
 // No menu 
 function blockContextMenu() {
     event.cancelBubble = true;
	 event.returnValue = false;
     return false;
 }
 // Backup no back 
 function blockKeyPresses() {
     var keyBackspace = 8;
     var keyF5 = 116;
     if ((event.keyCode == keyBackspace) || (event.keyCode == keyF5)) {
         event.keyCode = 0;
         event.returnValue = false;
     }
 }

