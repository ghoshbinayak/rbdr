


$(document).ready(function(){
	slideShow();
});
/*function Slider(){
			$("#slider #1").show("slide",{direction:"right"},500);
			$("#slider #1").delay(2500).hide("slide",{direction:"left"},500);
			var sc=$("#slider img").size();
			var count=2;
	
		setInterval(function(){
			$("#slider #"+count).show("slide",{direction:"right"},500);
			$("#slider #"+count).delay(2500).hide("slide",{direction:"left"},500);
			if(count==sc)
			{
				count=1;
			}
			else{
				count=count+1;
			}
			},4000);
		}*/
function slideShow() {
  
  var showing = $('#wrap #1');
  var sc=6;
  var count=1;
  var next = $("#wrap #"+count);
  showing.fadeOut(0, function() { next.fadeIn(500).addClass('is-showing'); }).removeClass('is-showing');
  /*
  * We could have written the above statement the long way to achieve the same results
  *
  * if(showing.next().length) {
  *		showing.next();
  *	} else {
  *		showing.parent().children(':first');
  * }
  */ 
  count=count+1;
	setInterval(function(){
			var next = $("#wrap #"+count);
			showing.fadeOut(500, function() { next.fadeIn(500).addClass('is-showing'); }).removeClass('is-showing');
			if(count==sc)
			{
				//count=1;
				
			}
			else{
				count=count+1;
			}
			showing=next;
  },1000);
  
}
