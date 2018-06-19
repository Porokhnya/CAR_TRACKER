jQuery(document).ready(function(){

jQuery(".niceCheck").mousedown(

function() {

     changeCheck(jQuery(this));
     
});


jQuery(".niceCheck").each(

function() {
     
     changeCheckStart(jQuery(this));
     
});

                                        });

function changeCheck(el)

{
     var el = el,
          input = el.find("input").eq(0);
   	 if(!input.attr("checked")) {
		el.css("background-position","0 -14px");	
		input.attr("checked", true);
		el.parent().addClass('active');
		$('li.none-search-bg').fadeOut();
		$('div.count_all').fadeOut('fast');
		$('div.count_active').fadeIn('fast');

	} else {
		el.css("background-position","0 0");	
		input.attr("checked", false);
		el.parent().removeClass('active');
		$('li.none-search-bg').fadeIn();
		$('div.count_all').fadeIn('fast');
		$('div.count_active').fadeOut('fast');
	}
     return true;
}

function changeCheckStart(el)

{
var el = el,
		input = el.find("input").eq(0);
      if(input.attr("checked")) {
			el.css("background-position","0 -14px");
			$('li.none-search-bg').fadeOut();
			$('div.count_all').fadeOut('fast');
			$('div.count_active').fadeIn('fast');
		}else{
			$('li.none-search-bg').fadeIn();
			$('div.count_all').fadeIn('fast');
			$('div.count_active').fadeOut('fast');
		}
     return true;
}