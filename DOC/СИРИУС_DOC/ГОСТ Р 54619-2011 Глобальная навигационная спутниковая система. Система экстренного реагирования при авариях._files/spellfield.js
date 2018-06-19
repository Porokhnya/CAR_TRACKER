$(document).ready(function(){
    var url = "/spellfield/";
    /*var items = new Array();
    var types = new Array();
    var numbers = new Array();*/
    var items = false;
//	localStorage.clear();
	var current_item = null;

	$.cookie.json = true;
	$.cookie.defaults = {
		expires: 30 * 24 * 60 * 60,
		path: '/'
	};

	// Удаление запроса, если результатов поиска нет
	var queries = $.cookie('search_queries');
	if(queries && $('div.notFound').length > 0){
		queries = $.grep(queries, function(item) {
			return item.value != $('input#search-field').val();
		});
		$.cookie('search_queries', queries);
	}

	// Отсюда: http://stackoverflow.com/questions/280793/case-insensitive-string-replacement-in-javascript
	function preg_quote( str ) {
		// http://kevin.vanzonneveld.net
		// +   original by: booeyOH
		// +   improved by: Ates Goral (http://magnetiq.com)
		// +   improved by: Kevin van Zonneveld (http://kevin.vanzonneveld.net)
		// +   bugfixed by: Onno Marsman
		// *     example 1: preg_quote("$40");
		// *     returns 1: '\$40'
		// *     example 2: preg_quote("*RRRING* Hello?");
		// *     returns 2: '\*RRRING\* Hello\?'
		// *     example 3: preg_quote("\\.+*?[^]$(){}=!<>|:");
		// *     returns 3: '\\\.\+\*\?\[\^\]\$\(\)\{\}\=\!\<\>\|\:'

		return (str+'').replace(/([\\\.\+\*\?\[\^\]\$\(\)\{\}\=\!\<\>\|\:])/g, "\\$1");
	}

	$('#search-field').before('<span id="input_placeholder"></span>');
	$('#input_placeholder').before('<div id="input_placeholder_hidden" style="display: none; font-size: 16px; white-space: pre;"></div>');

	var placeholder_text = '';
	$('#search-field').bind('keyup', function(e) {

		if(e.keyCode == 39 && placeholder_text) {
			$(this).val(placeholder_text);
		}

		var val = $(this).val();
		setInputPlaceholder(placeholder_text, val);
	});

	$('#search-field').bind('blur', function() {
		$('#input_placeholder').text('');
		$('#input_placeholder_hidden').text('');
	});

	$('#search-field').bind('focus', function() {
		var val = $(this).val();
		setInputPlaceholder(placeholder_text, val);
	});

	var setInputPlaceholder = function(text, val) {


//		if($('#input_placeholder_hidden').width() > $('#search-field').width()){
//			$('#input_placeholder').html('');
//			$('#input_placeholder').css('margin-left', '0px');
//			return;
//		}
		if(val == '') {
			$('#input_placeholder').html('');
			return;
		}
		if(text.toLowerCase().substring(0, val.length) == val.toLowerCase()) {
			$('#input_placeholder_hidden').text(val);


//			text = text.replace(new RegExp('('+preg_quote(val)+')', 'gi'), "<span style='visibility: hidden;'>$1</span>");
			$('#input_placeholder').html(text.substring(val.length, text.length));

			if($('#input_placeholder_hidden').width() < $('#search-field').width())
				$('#input_placeholder').css('margin-left', (17 + $('#input_placeholder_hidden').width()) + 'px');
		}
		else {
			$('#input_placeholder').html('');
		}

//		$('#input_placeholder').css('text-overflow', 'ellipsis');
		var max_length = $('#search-field').width() - 70 - $('#input_placeholder_hidden').width() + 37;
//		if($('#input_placeholder').width() > max_length) {
			$('#input_placeholder').width(max_length);
	//	console.log(max_length);
//		}
	};

	var autocompleteRequest = null;
    $("#search-field").autocomplete(                
    {
        source: function( request, response ) {
		var val = $("#search-field").val();
	        if(autocompleteRequest) autocompleteRequest.abort();
	        autocompleteRequest = $.ajax({
			    url: url,
			    type: "post",
			    dataType: 'json',
			    data: "find=1&val="+val,
			    success: function(data){
	                /*for(var l = 0; l < data.length; l++) {
	                    items[l]    = data[l]['value'];
	                    types[l]    = data[l]['itemtype'];
	                    numbers[l]  = data[l]['itemnum'];
	                }*/

				    // Функция, удаляющая лишнее из массива
				    var itemsUnique = function (array) {
					    var a = array.concat();
					    for(var i=0; i<a.length; ++i) {
						    for(var j=i+1; j<a.length; ++j) {
							    if(a[i]['value'] === a[j]['value'])
								    a.splice(j--, 1);
						    }
					    }

					    return a;
				    };
				    var queries = $.cookie('search_queries');
	                $('#search-autocomplete .ui-widget-content').scrollTo(0, 1);

				    if(queries) {
					    var current_time = new Date().getTime() / 1000;

					    // Отсеиваем недельной давности
					    queries = $.grep(queries, function(item) {
						    return item.valid_till >= current_time/* && parseInt(item.counter) >= 1*/;
					    });

					    // Сохраняем
					    $.cookie('search_queries', queries);

					    // Убираем те, по которым кликнули меньше двух раз
					    queries = $.grep(queries, function(item) {
						    return parseInt(item.counter) >= 1;
					    });
	                    // Убираем то, что в локал сторадже не подходит под запрос
	                    queries = $.grep(queries, function(item) {
	                        return (item.value.toUpperCase().indexOf(val.toUpperCase()) + 1)==1 &&(val.length>1);
	                    });
					    data = itemsUnique(queries.reverse().concat(data));
				    }
	                items = data;
				    response(data);

				    $('#search-autocomplete .ui-widget-content li').each(function(){
					    var text = $(this).find('a').text();
					    text = text.replace(new RegExp('('+preg_quote(val)+')', 'gi'), "<span style='font-weight: normal'>$1</span>");
					    $(this).find('a').html('<strong>' + text + '</strong>');
				    });

				    placeholder_text = $('#search-autocomplete .ui-widget-content li').first().text().replace(/<\/?[^>]+(>|$)/g, "");
				    setInputPlaceholder(placeholder_text, val);

				    if(!queries) return;
//				    var queries_values = $.map(queries, function(item){ return item.value; });

			    }
			});
	    },
	    /*focus: function(){
		 var val = $(".ui-state-hover").html();
		 $("#search-field").val(val);
	    },*/
	    select: function(){            
        var val = $(".ui-state-hover").text();
         $("#search-field").val(val);
         for(var k=0; k < items.length; k++) {
            if(items[k]["value"] == val) {
	            current_item = items[k];
                if(items[k]["itemtype"] == "document") {
	                setUserQueries(current_item);
                    location.href = "/document/" + items[k]["itemnum"] + "/";
                    return false;
                }else{
					 $('input#itemtype').val(items[k]["itemtype"]);	
				}
			/*	if(items[k]["itemtype"] == "helper") {
                    $('input#itemtype').val("helper");
                }
			*/
            }
         }
		    $('#input_placeholder').text('');
		    $('#input_placeholder_hidden').text('');
//		$("#search-form").submit();
		    $(".search-box-submit").click();
	    }, 
		appendTo: "#search-autocomplete",
	    delay: 0
	}
    );

    function isScrolledIntoView(container, elem)
    {
        var docViewTop = container.scrollTop();
        var docViewBottom = docViewTop + container.height();

        var elemTop = $(elem).offset().top;
        var elemBottom = elemTop + $(elem).height();

        return ((elemBottom <= docViewBottom) && (elemTop >= docViewTop));
    }

    $("#search-field").keydown(function(e) {
        var container = $('#search-autocomplete .ui-widget-content');
        switch(e.keyCode) {
            case 38: case 40:
                e.preventDefault();
                if(container.height() > $('#search-autocomplete').height())
                {
                    var activeElement = null;
                    var total = 0;
                    container.find('li').each(function(){
                        total += 1;
                        if($(this).find('a').hasClass('ui-state-hover'))
                        {
                            activeElement = $(this);
                        }
                    });

                    if(e.keyCode == 40)
                    {
                        // Если достигли конца
                        if(activeElement == null)
                        {
                            container.scrollTo($('#search-autocomplete .ui-widget-content li').find(':first-child'));
                        }
                        // Если активный элемент находится в самом низу
                        else if(container.height() - activeElement.height() <= activeElement.position().top)
                        {
                            container.scrollTo(activeElement, {offset : {top: activeElement.height() - container.height()}});
                        }
                    }
                    else
                    {
                        // Если достигли начала
                        if(activeElement == null)
                        {
                            container.scrollTo($('#search-autocomplete .ui-widget-content li:last-child'));
                        }
                        // Если активный элемент находится в самом верху
                        else if(/*activeElement.position().top <= 0 &&*/ activeElement != null)
                        {
                            container.scrollTo(activeElement);
                        }
                    }
                }

                break;
        }
    });

    // Открываем форму 1 кликом
    $("#search-field").click(function(){
		if($("#search-field").val().length>0){
			if($(".ui-menu").is("ul") && $(".ui-menu").html() != ""){
				if($(".ui-menu").css('display') == 'none'){
					$(".ui-menu").css({'display':'block'});
				}
			}
		}
    });
    // Закрываем форму 2 кликом
    $("#search-field").dblclick(function(){
		if($("#search-field").val().length>0){
			if($(".ui-menu").is("ul")){
				if($(".ui-menu").css('display') != 'none'){
					$(".ui-menu").css('display','none');
				}            
			}
		}
    });  
	
	/*var title = 'Поиск';
	//$('#search-field').attr('title', title);
	if ($('#search-field').attr('value') === '') $('#search-field').attr('value', title);
	
    $('#hint span').bind('click', function(){
        var title = $(this).text();
		$('#search-field').attr('value', title);
        $('#search-field').attr('title', title);
    });
    
	$('#search-field').bind('focus', function() {
		if ($('#search-field').attr('value') === title) {
			$('#search-field').attr('value', '');
			$('#search-field').css('color', '#333'); //#979696
			$('#search-field').css('font-style', 'normal'); // italic
			$('#babl').animate({top: '-35px'});
		}
		
	});
	$('#search-field').bind('blur', function() {
		if ($('#search-field').attr('value') === '') {
			$('#search-field').attr('value', title); 
			$('#search-field').css('color', '#979696'); 
			$('#search-field').css('font-style', 'italic'); 
			$('#babl').animate({top: '-20px'});
		}
	});*/
    
    
    var title = 'Поиск';
    $('#search-field').val(title);
    
    $('#hint span').bind('click', function(){
        var title = $(this).text();
		$('#search-field').val(title);
        $('#search-field').attr('title', title);
    });
  
    $('#search-field').blur(function(){
      if ($(this).val() == '' || $(this).val() == 'Поиск'){
          $(this).val(title).css({'color':'#979696', 'font-style':'italic'});
          $('#babl').animate({top: '-20px'});
      }
      if ($(this).val() !== title && $(this).val() !== '') {
          $(this).css({'color':'#333', 'font-style':'normal'});
      }
    });

    $('#search-field').focus(function(){
       if ($(this).val() == title){
           $(this).val('').css({'color':'#333', 'font-style':'normal'});
           $('#babl').animate({top: '-35px'});
       }
    });
    
	
/*
	$("#search-field").keyup(function(event) {
		var e = event || window.event;
	    if (e.keyCode === 13) {

		if(items === false){
			var val = $("#search-field").val();
				$.ajax({
					url: url, 
					type: "post",
					dataType: 'json',
					data: "find=1&val="+val,
					success: function(data){
						items = data;
						for(var k=0; k < items.length; k++) {
						  if(items[k]["value"] == val) {
							  if(items[k]["itemtype"] == "document") {
								  location.href = "/document/" + items[k]["itemnum"] + "/";
								  return false;
							  }
							  if(items[k]["itemtype"] == "helper") {
								  $('input#itemtype').val("helper");
							  }
						  }
					   }
						$("#search-form").submit();
					
					},
					error : function(){	
						$("#search-form").submit();	
					}
				});
		}else{
			var val = $("#search-field").val();
			for(var k=0; k < items.length; k++) {
				if(items[k]["value"] == val) {
					if(items[k]["itemtype"] == "document") {
						location.href = "/document/" + items[k]["itemnum"] + "/";
						return false;
					}
					if(items[k]["itemtype"] == "helper") {
						$('input#itemtype').val("helper");
					}
				}
			 }
			 $("#search-form").submit();
		}

	    }
	});
*/
	$('#search-form').submit(function(e){	
//			if(items === false){
//				e.preventDefault();
//			}
		current_item = { value: $('input#search-field').val(), itemtype: '', itemnum: 0 };
//		console.log(current_item);
		setUserQueries(current_item);
	});


	$("#search-field").keyup(function(event) {
		var e = event || window.event;
	    if (e.keyCode === 13) {
		    var item = null;
	        var val = $(this).val();
			for(var k=0; k < items.length; k++) {
			    if(items[k]["value"] == val) {
				    current_item = items[k];
				    if(items[k]["itemtype"] == "document") {
					    setUserQueries(current_item);
					    location.href = "/document/" + items[k]["itemnum"] + "/";
					    return false;
				    }else{
					   $('input#itemtype').val(items[k]["itemtype"]);
				    }
			    }
            }
//	        $("#search-form").submit();
	        $(".search-box-submit").click();
	    }
	});


});
function setUserQueries(item){
	if(!item) return;

	item = $.extend({}, item);

	item.valid_till = new Date().getTime() / 1000 + 7 * 24 * 60 * 60;
	item.counter = 0;

	var time = new Date().getTime() / 1000 + 7 * 24 * 60 * 60;

	var queries = $.cookie('search_queries');
//	console.log($.map(queries, function(item){ return item.value + ' - ' + item.counter; }));
	if(!queries) queries = [];

	// Получаем список самих запросов
	var queries_values = $.map(queries, function(item){ return item.value; });

	var index = $.inArray(item['value'], queries_values);
	// Если нет такого запроса, сохраняем его
	if(index === -1){
//		if(queries.length == 3) queries.splice(0, 1);
		queries.push(item);
	}
	// Иначе увеличиваем счетщик, если прошло 2 сек с момента последнего запроса
	else if(time - queries[index].valid_till >= 2) {
		queries[index].counter += 1;
		queries[index].valid_till = time;
	}

	$.cookie('search_queries', queries);
}