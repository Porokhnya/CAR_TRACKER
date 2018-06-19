//----------------Поисковая фича для содержания-----------------//
	var countSoderj = 0;
    function doDropDownSoderj(){
       countSoderj = countSoderj+1;
       setTimeout("dropDownGoSoderj("+countSoderj+")",300);
    }

    function dropDownGoSoderj(currCount){
		var lock;
		if(currCount == countSoderj)
		{
			if (!lock) {
				lock = true;
				setTimeout(function(){
					lock = false;
					if(currCount == countSoderj)
					{
						countSoderj = 0;
						 searchSoderjanie();
					}
				},400)
			}
		}
    }

//----------------Поисковая фича для текста-----------------//
    var countText = 0;
    function doDropDownText(){
       countText = countText+1;
       setTimeout("dropDownGoText("+countText+")",300);
    }

    function dropDownGoText(currCount){
		var lock;
		if(currCount == countText)
		{
			if (!lock) {
				lock = true;
				setTimeout(function(){
					lock = false;
					if(currCount == countText)
					{
						countText = 0;
						 searchText();
					}
				},400)
			}
		}
    }

$(document).ready(function(){
	//Регстронезависимый поиск
	$.extend($.expr[':'], {
		'containsi': function(elem, i, match, array)
		{
			return (elem.textContent || elem.innerText || '').toLowerCase()
			.indexOf((match[3] || "").toLowerCase()) >= 0;
		}
	});

	//по нажатию ищем следующие вхождения
	entrySearchSoderjanie = 1;
	entrySearchText = 1;
	$('#search_window a.search_form_pic').live('click', function(event){
		event.preventDefault();
		entryText();
	});

	$('#content_field .search_form_pic').live('click', function(event){
		event.preventDefault();
		entrySoderj();
	});

	//Если нажали ентер на текстовом поле содержания
	$('#content_field .search_form').live('keyup', function(e)
	{
		if(e.keyCode==13){
			entrySoderj();
		}else{
			doDropDownSoderj();
		}
	});

	//Если нажали ентер на текстовом поле поиска по тексту
	$('#search_form .search_form2').live('keyup', function(e)
	{
		if(e.keyCode==13){
			entryText();
		}else{
			doDropDownText();
		}
	});


});

// ищем следующие вхождения (выделяем бордером)
function entrySoderj(){
	var entry = false;

	$('.searchSoderjanie').css('border','');
	if($('.searchSoderjanie').length){
		$.each($('.searchSoderjanie'), function(key,value){
			if(key == entrySearchSoderjanie){
				$('#soderjanie').scrollTo(value, 400, {axis:'y', offset: {top:-80} } );
				$(value).css('border', '1px solid');
				entry = true;
			}
		});

		//ничего не нашло, обнуляем счетчик, показываем первое вхождение (нулевое)
		if(!entry){
			entrySearchSoderjanie = 1;
			$('#soderjanie').scrollTo($('.searchSoderjanie:first'), 400, {axis:'y', offset: {top:-80}} );
			$('.searchSoderjanie:first').css('border', '1px solid');
		}else{
			entrySearchSoderjanie++;
		}
	}else{
		searchSoderjanie();
	}
}

// ищем следующие вхождения (выделяем бордером)
function entryText(){
	var entry = false;

	$('.searchText').css('border','');
	if($('.searchText').length){
		$.each($('.searchText'), function(key,value){
			if(key == entrySearchText){				
				$('body').scrollTo(value, 800, {axis:'y', offset: {top:-300}} );
				$(value).css('border', '1px solid');
				entry = true;
			}
		});

	//ничего не нашло, обнуляем счетчик, показываем первое вхождение (нулевое)
		if(!entry){
			entrySearchText = 1;
			$('body').scrollTo($('.searchText:first'), 800, {axis:'y', offset: {top:-300}} );
			$('.searchText:first').css('border', '1px solid');
		}else{
			entrySearchText++;
		}
	}else{
		searchText();
	}
}

function highlight( word,line,triger ){
	
	var clas = '';
	if(triger == 'text'){
		clas = 'searchText';
	}else if(triger == 'soderj'){
		clas = 'searchSoderjanie';
	}
	
	var regex = new RegExp('(' + word + ')', 'gi');

	var match = line.match(/(<[^>]+>)?([^<]+)?/g);
	var str = '';
	$.each(match, function(key,value){
		if(value != ''){
			var match2 = value.match(/(<[^>]+>)?([^<]+)?/);
			$.each(match2, function(key2,value2){
				if(key2 > 0 && typeof(value2) != 'undefined'){
					if(key2 == 2){
						value2 = value2.replace(regex, '<span class="'+clas+'" style="background-color:yellow">$1</span>');
					}

					if(value2 != undefined && value2 != ''){
						str =str + value2;

					}
				}
			});
		}
		

	});
//	line.replace(/(<[^>]+>)?([^<]+)(<[^>]+>)?/i, function(m, key, value){
	//	console.log(m, key, value);
//	});
	return str;
	if(triger == 'text'){		
		return line.replace(regex, '<span class="searchText" style="background-color:yellow">$1</span>');
	}
	if(triger == 'soderj'){		
		return line.replace(regex, '<span class="searchSoderjanie" style="background-color:yellow">$1</span>');
	}
}




function searchSoderjanie(){
	//Все обнуляем
	entrySearchSoderjanie = 1;
	var searchStr = $.trim($('input.search_form').val());
	$('.search_form').css('background-color', '');
	$('div#soderjanie ul.soderjanie ul').slideUp(400, function(){
		$(this).parent().find('div.arrow:first').removeClass('ar_opened').addClass('ar_closed');
		$(this).removeClass('open_ul');
	});
	
	$('div#soderjanie ul.soderjanie span.searchSoderjanie').contents().unwrap();
	
	if(searchStr != ''){
		var text = $('div#soderjanie ul.soderjanie a:containsi('+searchStr+')');

		var found = false;

		$.each(text, function(key,value){
			found = true;
			
			//Подсвечиваем строку
			$(value).html(highlight(searchStr ,$(value).html(),'soderj'));
			
			var ul = value;
			//защита от зацыкливания
			var i=0;
			
			//Если ul еще не открыт
			if($(ul).parent().closest('ul').attr('class') != 'open_ul'){
				//Рекурсивно обходим верхние ul и открываем их.
				while(ul = recursion(ul) || i == 10){
					$(ul).addClass('open_ul');
					$(ul).slideDown(300,function(){
						$(this).parent().find('div.arrow:first').removeClass('ar_closed').addClass('ar_opened');		
					});
					i++;
				}	
			}

			if(key == 0){
				setTimeout(function(){
					$('#soderjanie').scrollTo(value, 100, {axis:'y', offset: {top:-80}} );
				}, 500);
			}
		});

		if(!found){
			$('.search_form').css('background-color', '#dc4141');
		}

	}
}

//Рекурсивно перебираем элементы
function recursion(ul_start){
	var ul			= $(ul_start).parent().closest('ul', 'div#soderjanie');
	var soderj_div	= $(ul).parent();
	
	if($(soderj_div).attr('id') == 'soderjanie'){
		return false;
	}
	return ul;
}

function searchText(){
	entrySearchText = 1;
	var searchStr = $.trim($('input.search_form2').val());
	$('.search_form2').css('background-color', '');

	$('#doc_with_soderjanie span.searchText').contents().unwrap();
	if(searchStr != ''){
		var text = $(
            '#doc_with_soderjanie p:containsi('+searchStr+'), ' +
            '#doc_with_soderjanie h5:containsi('+searchStr+'), ' +
            '#doc_with_soderjanie h4:containsi('+searchStr+'), ' +
            '#doc_with_soderjanie h3:containsi('+searchStr+'), ' +
            '#doc_with_soderjanie h2:containsi('+searchStr+'), ' +
            '#doc_with_soderjanie h1:containsi('+searchStr+')'
            );

		var found = false;

		$.each(text, function(key,value){
			found = true;

			 $(value).html(highlight(searchStr ,$(value).html(), 'text'));

			if(key == 0){
				setTimeout(function(){
					$('body').scrollTo(value, 800, {axis:'y', offset: {top:-300}} );
				}, 500);
			}
		});

		if(!found){
			$('.search_form2').css('background-color', '#dc4141');
		}

	}
}