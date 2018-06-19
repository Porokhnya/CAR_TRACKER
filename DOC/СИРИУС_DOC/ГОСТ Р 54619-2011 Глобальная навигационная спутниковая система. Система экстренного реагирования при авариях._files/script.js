$(window).load(function() {
	$('#h-tabs').css('display','block');

});

// возвращает cookie с именем name, если есть, если нет, то undefined
function getCookie(name) {
	var matches = document.cookie.match(new RegExp(
		"(?:^|; )" + name.replace(/([\.$?*|{}\(\)\[\]\\\/\+^])/g, '\\$1') + "=([^;]*)"
	));
	return matches ? decodeURIComponent(matches[1]) : undefined;
}




$(document).ready(function(){
	$("html,body").scrollTop(1);

	window.onload = function() { // нафига onload ттут описано здесь http://www.belaychuk.ru/posts/yandex-metrika-reachgoal-hit-gtm и тут http://stackoverflow.com/questions/3698200/window-onload-vs-document-ready
	var eventStart = false;
	var eventFree = false;

	 // var yaCounter5998651 = new Ya.Metrika({id: 5998651, enableAll: true});

	//$.when(yaCounter5998651.reachGoal('Banner2_Techexpert')).then(function(){alert("LOL!!!")});
	//yaCounter5998651

$('.bannerButtonApple3').live('click',(function (event) {
	var _this = this;
	if (!eventStart) {
		eventStart = true;
		$.when(yaCounter5998651.reachGoal('Banner2_Techexpert')).then(function () {
			_this.click();
		})
		return false;
	}
}));


$('.bannerButtonApple4').live('click',(function (event) {
	var _this = this;
	if (!eventStart) {
		eventStart = true;
		$.when(yaCounter5998651.reachGoal('Banner2_Kodeks')).then(function () {
			_this.click();
		})

		return false;
	}
}));


	$('.bannerButtonApple1').live('click',(function (event) {
		var _this = this;
		if (!eventStart) {
			eventStart = true;
			$.when(yaCounter5998651.reachGoal('Banner1_Kodeks')).then(function () {
				_this.click();
			})

			return false;
		}
	}));


	$('.bannerButtonApple2').live('click',(function (event) {
		var _this = this;
		if (!eventStart) {
			eventStart = true;
			$.when(yaCounter5998651.reachGoal('Banner1_Techexpert')).then(function () {
				_this.click();
			})

			return false;
		}
	}));

// выпадашка
$ ( function() {

		$('.tab_i').on('click touchstart', function(event) {
			var inner = $(this).children();
				if (inner.hasClass('__active')) {
						inner.removeClass('__active');
						inner.fadeOut(300);
				}
				else{
						$('.tab_i').children().fadeOut(300);
						$('.tab_i').children().removeClass('__active');
						inner.fadeIn(300);
						inner.addClass('__active');
				}
				event.stopPropagation();

		});



		$('.modalbox-app').on('touchstart click',function(e){
			e.stopPropagation();
		})
});



$(document).on('click touchstart',function(event) {
		if ($(event.target).children(".modalbox-app").length)	return;
		$('.modalbox-app').fadeOut(300);
		$('.modalbox-app').removeClass('__active');
		event.stopPropagation();
});


// /выпадашка
	var cooc = getCookie('ipadAbCooc'); // получаем куку

	var isApple =  (/ipad/gi).test(navigator.appVersion);

	if(isApple){
		if( cooc !='show' || cooc == undefined ){
			//alert('No Coocies!');
			//setTimeout(function(){
			//$('#content_wrapper').fancybox({
			$.fancybox.open([],{
				//maxWidth    : 540,
				padding		: 0,
				fitToView    : false,
				 // width        : '90%',
				 // height        : '75%',
				autoSize    : true,
				closeClick    : false,
				openEffect    : 'fade',
				'scrolling' : 'no',
				href        : '/user/ipadab',
				type        : 'ajax',
				beforeShow : function() {
					$fi = $('.fancybox-inner')
					ph = $fi.height()
					pw = $fi.width()
					this.title = null;

					//location.hash = '#ipad_promo';
				},
				afterShow : function() {
					fi = $('#ipadAbtestBanner');
					ph = fi.height();
					pw = fi.width();
					$('.fancybox-inner').height(ph);
					$('.fancybox-inner').width(pw);
				},
				afterClose: function () {
					location.hash = '';
				}


			}); // fancybox end
			$("#content_wrapper").trigger('click'); // Вызов при загрузке
			var date = new Date( new Date().getTime() + 86400*1000 ); // Кладем куку на сутки
			document.cookie="ipadAbCooc=show; path=/; expires="+date.toUTCString();
		} // проверка на куку
	} // ПРоверка на яблочко

	var isAndroid =  (/android/gi).test(navigator.appVersion);

	if(isAndroid){ //Костыль для Андроида

		var cooc = getCookie('androidAbCooc'); // получаем куку
		if( cooc !='show' || cooc == undefined ) {
			$.fancybox.open([], {
				//maxWidth    : 540,
				padding: 0,
				fitToView: false,
				// width        : '90%',
				// height        : '75%',
				autoSize: true,
				closeClick: false,
				openEffect: 'fade',
				'scrolling': 'no',
				href: '/user/androidab',
				type: 'ajax',
				beforeShow: function () {
					$fi = $('.fancybox-inner')
					ph = $fi.height()
					pw = $fi.width()
					this.title = null;
					$('.fancybox-inner').on('click', '.subscribe-button', function () {
						var email = $('.fancybox-inner input.subscribe-email').val();

						$.ajax({
							url: '/user/androidsubscr',
							data: {email: email},
							success: function (response) {
								$('.fancybox-inner .description').hide();
								$('.fancybox-inner .subscribe').hide();
								$('.fancybox-inner .complete').show();
							}, error: function (xhr, textStatus, errorThrown) {

							}
						});
					});

					//location.hash = '#ipad_promo';
				},
				afterShow: function () {
					fi = $('#androidAbtestBanner');
					ph = fi.height();
					pw = fi.width();
					$('.fancybox-inner').height(ph);
					$('.fancybox-inner').width(pw);
				},
				afterClose: function () {
					location.hash = '';
				}


			}); // fancybox end

			var date = new Date( new Date().getTime() + 86400*1000 ); // Кладем куку на сутки
			document.cookie="androidAbCooc=show; path=/; expires="+date.toUTCString();
		}
	}
	}
}); // Document Ready



function setEqualHeight(columns) {
 columns.css('height', 'auto');
 var tallestcolumn = 0;
 columns.each(function() {
	currentHeight = $(this).height();

	if(currentHeight > tallestcolumn) {
	 tallestcolumn = currentHeight;

	}
 });
 columns.css('height', tallestcolumn);
 bloks(tallestcolumn);

 $(window).resize(function() {
	columns.css('height', 'auto');

	var tallestcolumn = 0;
	 columns.each(function() {
	currentHeight = $(this).height();

	if(currentHeight > tallestcolumn) {
	 tallestcolumn = currentHeight;

	}
	 });
	columns.css('height', tallestcolumn);
	bloks(tallestcolumn);
	//хлебные крошки под размер окна.
	breed();

	//Банер судебной практики
	jurisprudence();

	//important();

 });
}

//поведение важных документов при ресайзе
/*function important(){
	//Левый нижний
	var width_te_n = $('td.te').width() * 2 - 72;
	$('#important_docs_dl').css('margin-left', width_te_n);

	//Левый верхний
	var width_te_v = $('th.th1').width();
	$('#important_docs_tl').css('left', width_te_v);

	//Правый верхний
	var width_te_v2 =  width_te_v*2 + $('th.th2').width() + $('#top-te td').width();
	$('#important_docs_tr').css('margin-left', width_te_v2);

	//Правый нижний
	var width_te_n2 =  $('td.te').width()*2 + $('td.ko').width()*2;
	$('#important_docs_dr').css('margin-left', width_te_n2);

}*/


//блоки на главной
function bloks(tallestcolumn){
	//блоки
	var left = 182;
	var top = 346;

	top = tallestcolumn-38;
	var table_w = $('#left_table').width();
	left = (table_w-252)/2 ;
	if(left < 160){

		$(".floater_inner").css('height', tallestcolumn+38);
		 top = tallestcolumn;
	}

	$('#l_pi').css({'left': left});
	$('#r_pi').css({'left': left});

/*
	//картинка техэксперта
	var te_wi = $('.floater.te').width();
	$('.pen_t').css('left', te_wi-320);

	//картинка кодекса
	var te_ki = $('.floater.ko').width();
	$('.pen_k').css('left', te_ki-340);*/

	}


function breed(){
	//хлебные крошки под размер окна. НЕ ИСПОЛЬЗУЕТСЯ
	/*
	var widthHead = $('#top-menu').width();

	var widhtLi = 0;
	$.each($('#bottom-header li'), function(a,b){
			widhtLi =  widhtLi + $(b).width();
	});
	console.log(widhtLi);
	console.log(widthHead);

	if(widthHead < widhtLi){
		if(widhtLi > 800){
		$('#bottom-header').css('height', 45);
		}
	}else{
		 $('#bottom-header').css('height', 30);
	}
	*/
}

//сдвиг банера юриспрюнеднции по высоте
function jurisprudence(){
	var width = $('#page-wrapper').width();
	width = ((width - 770) /2-188)+770; //На сколько сдивнуть блок в зависимости от ширины
	$('#jur_banner').css('margin-left', width);

	var body = $('body').height();
	var height = body - 279;
	 $('#jur_banner').css({'bottom' : height});
}

/*function showTablow(tabName){
	//console.log("tabName: "+tabName);
	if(tabName == 'content-low' && $('#search_and_content').is('div')){
		$('#search_and_content').show();
	}else if($('#search_and_content').is('div')){
		$('#search_and_content').hide();
	}
	//скрываем все табы
	$('#tabs-selector-low > div').removeClass('current-low');
	$('#tabs-content-low > div').hide();
	//показываем нужный таб
	$('#switch-' + tabName).addClass('current-low');
	$('#tab-' + tabName).show();
}*/

function showTabsearch(tabName){
	//скрываем все табы
	$('#tabs-search > div').removeClass('current-search');
	$('#tabs-content-search > div').hide();
	//показываем нужный таб
	$('#switch-' + tabName).addClass('current-search');
	$('#tab-' + tabName).show();
}

function backTop(){
	var pageWrapperHeight = $('#page-wrapper').height();
	if($('a').is('#nextpage')){
		var smesh = 400;
	}else{
		var smesh = 690;
	}
	var margin = $('#block bottom').height() + $('#top-menu').height() + smesh;
	var bannerFlag =  !document.getElementById("promoBanner"); // Если баннер есть то вернет false
	var bannerDisplay = $('#promoBanner').css('display');


	if((!bannerFlag && bannerDisplay == 'none') || bannerFlag ){ // проверка на то стоитт ли скрывать кнопку наверх
		if ($(this).scrollTop() > 230 && ($(this).scrollTop() < (pageWrapperHeight - margin))) {
			$('#back-top').fadeIn();
			$('#back-top').css({'position' : 'fixed', 'bottom' : '30px'});
		} else {
			if($(this).scrollTop() > (pageWrapperHeight - margin)){
				$('#back-top').css({'position' : 'absolute', 'bottom' : '0'});
			}else if($(this).scrollTop() < 230) {
				$('#back-top').fadeOut();
			}
		}
	}

	// promo banner
	 /* var pageWrapperWidth = $('#page-wrapper').width()/2.5;
	if (pageWrapperWidth>390) {
		$('.promoBanner_content_wrap').css({'right' : -pageWrapperWidth});
	}
	$(window).resize(function() {
		var pageWrapperWidth = $('#page-wrapper').width()/2.5;
		if (pageWrapperWidth>390) {
		$('.promoBanner_content_wrap').css({'right' : -pageWrapperWidth});
		}
	});*/

	$('.shareBanner').css({bottom: '-110px'});
	if ($('.payBlockAssistant').length===1 & !$('.shareBannerWrapper').hasClass('on')) {
		if (!$('.shareBannerWrapper').hasClass('inTheButtom'))
		$('.shareBanner').fadeOut();
	}

	var doc_with_soderjanie = $('#doc_with_soderjanie'), jur_block_wrapper = $('#jur_block_wrapper');
		var updateTransition = function(){
			if ($(this).scrollTop() <= $(window).height()){
				$('.shareBanner').addClass('transition');
			}else{
				$('.shareBanner').removeClass('transition');
			}
		}
		if(doc_with_soderjanie.length===1){
			switch (true){
				case $(this).scrollTop() <= 7:
					updateTransition();
					$('.shareBanner').css({bottom: '-110px'});
					return;
				case $(window).scrollTop() + $(window).height() <= doc_with_soderjanie.height() + doc_with_soderjanie.offset().top + 110:
					$('.shareBannerWrapper').addClass('on');
					$('.shareBanner').fadeIn().css({bottom: 0});
					updateTransition();
					break;
				default:
					$('.shareBannerWrapper').removeClass('on');
					$('.shareBanner').css('bottom','-110px');
			}
		} else if(jur_block_wrapper.length===1){
			switch (true){
				case $(this).scrollTop() <= 7:
					updateTransition();
					$('.shareBanner').css({bottom: '-110px'});
					return;
				case $(window).scrollTop() + $(window).height() <= jur_block_wrapper.height() + jur_block_wrapper.offset().top + 110:
					$('.shareBannerWrapper').addClass('on');
					$('.shareBanner').fadeIn().css({bottom: 0});
					updateTransition();
					break;
				default:
					$('.shareBannerWrapper').removeClass('on');
					$('.shareBanner').css('bottom','0');
			}
		} else if ($('#tabs-selector-low').length){
			// для документов, предоставляемых без текста, только в виде карточек, например этот - http://test.cntd.ru/document/871100929
			updateTransition();
			$('.shareBanner').css({bottom: '-110px'});
		}
		if (950 < $('body').width() < $('#page-wrapper').width() & $(this).scrollLeft() > 0) {
		$('.shareBannerWrapper.on').css({left: -($(this).scrollLeft())});
		$('#back-top a').css({'margin-left': -($(this).scrollLeft())});
		} else {
		$('.shareBannerWrapper.on').css({left: 0});
		$('#back-top a').css({'margin-left': 0});
		}



	// close
	/*$('.shareBanner_content').hover(function() {
		$(this).addClass('plus');
	},
	function() {
		$(this).removeClass('plus');
	});*/

	$('.shareBanner_close').on('click', function() {
	 $('.shareBanner_content').fadeOut();
	});
}

/*
$(window).scroll(function() {
	if ( $(window).scrollTop() >= 18830 ) {
		$(window).scrollTop(201);
	}
	else if ( $(window).scrollTop() == 0 ) {
		$(window).scrollTop(18629);
	}
});
*/

$(document).ready(function(){

	var offset2 = $(".fixedblock_2").offset();

	$(".fixedblock_2").removeClass('on');
	$('.fixedblock_content').css({left: '-40px'});

	// иконки
	$(window).scroll(function(){
		if(!offset2) return;

		if ($(window).scrollTop() > offset2.top) {
			$(".fixedblock_2").addClass('on');
		} else {
			$(".fixedblock_2").removeClass('on');
			$('.fixedblock_content').css({left: '-40px'});
		}

		 if ($('body').width() < $('#page-wrapper').width() & $(this).scrollLeft() > 0) {
			$('.fixedblock_2').css({left: -($(this).scrollLeft())});
		 } else if ($('body').width() < $('#page-wrapper').width() & $('body').width() < 950) {
			$('.fixedblock_2.on').css({left: 0});
			$('.fixedblock_2.on .fixedblock_content').css({left: '60px'});
		 } else {
			$('.fixedblock_2.on').css({left: 0});
			$('.fixedblock_2.on .fixedblock_content').css({left: '-40px'});
		}
	});

	 $(window).resize(function() {
		 if(!offset2) return;
		$(".fixedblock_2").removeClass('on');
		$('.fixedblock_content').css({left: '-40px'});
		if ($(window).scrollTop() > offset2.top)
			$(".fixedblock_2").addClass('on');

		if ($('body').width() < $('#page-wrapper').width() & $('body').width() < 950) {
			$('.fixedblock_2.on').css({left: 0});
			$('.fixedblock_2.on .fixedblock_content').css({left: '60px'});
		 } else {
			$('.fixedblock_2.on').css({left: 0});
			$('.fixedblock_2.on .fixedblock_content').css({left: '-40px'});
		}
	});


	Cufon.replace(".partialAccess_link_caps, .partialAccess_link_low");

	// payment
	$(".buy_document").live('click', function() {
		$(".paymentBlock").slideToggle("fast");
		$('.shareBanner').hide();
	});

	$('body').on('click touchend', function(e) {
		if(!$(e.target).is('.switchTabs li'))
			$('.switchTabs').removeClass('visibleBlock');
	});

	// Для того, чтобы при возврате со страницы промотра документов мы попали в список документов, а не тематик.
	if (window.location.href == localStorage.getItem('switchTabs')){
		$('.switchTabs_item').first().removeClass('activeTab');
		$('.switchTabs_item').eq(1).addClass('activeTab');
		$("#tabs-content-low > div").hide();
		$("#tab-content-important-low").show();
	}

	// вкладки текст/статус
	$(".switchTabs_item").on("click", function(e) {
		var _this = $(this);
		var popID = _this.attr("rel");

		if (popID == 'content-important-low'){
			localStorage.setItem('switchTabs', window.location.href);
		}else{
			localStorage.removeItem('switchTabs');
		}

		$(this).parent().find('li').removeClass('activeTab');
		$(this).addClass('activeTab');
		if($('.switchTabs').hasClass('visibleBlock')) {
			$(".switchTabs").removeClass("visibleBlock");
			$("#tabs-content-low > div").hide();
			$("#tab-" + popID).show();

			// Смещаем баннер при переключении вкладок "статус" или "оперативная информация"
			if (popID == 'content-low' &&  $(window).scrollTop() > 7){
				$('.shareBannerWrapper').addClass('on');
				$('.shareBanner').fadeIn().css({bottom: 0});
				if ($(window).scrollTop() <= $(window).height()){
					$('.shareBanner').addClass('transition');
				}else{
					$('.shareBanner').removeClass('transition');
				}
				$('.shareBannerWrapper').removeClass('inTheButtom');
			}else{
				$('.shareBanner').css('display','');
				$('.shareBannerWrapper').removeClass('on');
				$('.shareBanner').removeClass('transition');
				$('.shareBannerWrapper').addClass('inTheButtom');

				$('.shareBanner').css('bottom','-110px');
			}
		}
		else {
			$(".switchTabs").addClass("visibleBlock");
		}


//        if ($(this).hasClass("activeTab")) {
//          $(".switchTabs").addClass("visibleBlock");
//          $(this).removeClass("activeTab");
//        } else {
//          $(".switchTabs").removeClass("visibleBlock");
//          $("#tabs-content-low > div").hide();
////          $(this).addClass("activeTab");
//          $("#tab-" + popID).show();
//        }

		// убираем кнопку поиск если активна вкладка статус
		if(popID == 'content-low' && $('#search_and_content').is('div')){
			$('#search_and_content, .search_and_content_block').show();
		}else if($('#search_and_content').is('div')){
			$('#search_and_content, .search_and_content_block').hide();
		}

		//Одни костыли в этом файле =( Беда...
		 /*   var pageWrapperHeight = $('#page-wrapper').height();
			if($('a').is('#nextpage')){
				var smesh = 400;
			}else{
				var smesh = 690;
			}
			var margin = $('#block bottom').height() + $('#top-menu').height() + smesh;

			if (($(window).scrollTop() < (pageWrapperHeight - margin))) {
				$('#promoBanner').css({'position' : 'fixed'});
			} else {
				if($(window).scrollTop() > (pageWrapperHeight - margin)){
					$('#promoBanner').css({'position' : 'absolute'});
				}
			}*/
	});





	// ссылка на оперативную информацию
		$("#oper_inf_link").on("click", function() {
		var popID = $(this).attr("rel");
		$("#tabs-content-low > div").hide();
		$(".switchTabs_item").removeClass("activeTab");
		$(".switchTabs_item"+"[rel="+popID+"]").addClass("activeTab");
		$("#tab-" + popID).show();
		$('.shareBannerWrapper').removeClass('on');
		});

	$(".hiddentab-low").hide();
	$(".hiddentab-loww").hide();


	var titleH1 = $("#tab-content2-low div.document h1").remove();
	$('#tab-content-low div.document hr.prepend').before(titleH1);


	// блоки кодекс и техэксперт на главной
	setEqualHeight($(".floater_inner"));

	//хлебные крошки под размер окна.
	breed();
	//Банер судебной практики
	jurisprudence();
	backTop();
	//important();

	// выпадающее меню
	$("a.important_docs").live('click', function(e) {
		var parentEl = $(this).parent().get(0);
		var linkEl = $(this).attr("href");
		var _this = this

		var up = $(this).attr('class');
		if(up == 'important_docs up'){
			$(this).removeClass('up');
			$(linkEl).hide();
			$(parentEl).removeClass('active');
		}else{
			$(this).addClass('up');
			$(parentEl).addClass('active');
			$(linkEl).show();
		}

		$(document).bind('click.myEvent.'+$.trim(linkEl)+' touchend.myEvent.'+$.trim(linkEl), function(e) {
			if ($(e.target).closest($(linkEl)).length == 0) {
				$(_this).removeClass('up');
				$(linkEl).hide();
				$(parentEl).removeClass('active');
				$(document).unbind('click.myEvent.'+$.trim(linkEl)+' touchend.myEvent.'+$.trim(linkEl));
			}
		})
		e.preventDefault();
	});

	// иконки социальных сетей
	$('.copyIcons ul li').each(function() {
		$(this).hover(function() {
			$(this).addClass('plus');
		},
		function() {
			$(this).removeClass('plus');
		});

	});


	//Выпадающее меню для важных документов
	/*$('a.important_doc').live('click', function(e){
		var attr = $(this).attr('href');

		var triger = $(attr).css('display');

		if(triger == 'none'){
			$(attr).fadeIn(100);
		}else{
			$(attr).fadeOut(100);
		}

			$(document).bind('click.myEvent', function(e) {
				if ($(e.target).closest($(attr)).length == 0) {
					$(attr).fadeOut(100);
					$(document).unbind('click.myEvent');
				}
			});

		e.preventDefault();
	})

	$('a.important_docs').live('click', function(e){
		var attr = $(this).attr('href');

		var triger = $(attr).css('display');

		if(triger == 'none'){
			$(attr).fadeIn(100);
		}else{
			$(attr).fadeOut(100);
		}

			$(document).bind('click.myEvent', function(e) {
				if ($(e.target).closest($(attr)).length == 0) {
					$(attr).fadeOut(100);
					$(document).unbind('click.myEvent');
				}
			});

		e.preventDefault();
	})*/

	// Кнопка наверх
	// fade in #back-top
	$(function () {
		$(window).scroll(function () {
			backTop();
		});

		// scroll body to 0px on click
		$('#back-top a').click(function () {
			$('body,html').animate({
				scrollTop: 0
			}, 800);
			return false;
		});
	});
	// **** Кнопка наверх



$('.search-box-submit').click(function(){

	var keyword = $('#search-field').val();
	if(keyword == 'Поиск'){
		var title = 'Введите запрос для поиска';
		$('#search-field').val(title);

		$('#search-field').focus(function(){
			 if ($(this).val() == title){
				 $(this).val('').css({'color':'#333', 'font-style':'normal'});
				 $('#babl').animate({top: '-35px'});
			 }
		});

		return false;
	}
	if(keyword == 'Введите запрос для поиска'){
		return false;
	}

});


	//название категории в зависимости от длинны
/*    var heightBlock = $('#search-title .request').height();

	if(heightBlock > 26 && heightBlock <= 51){
		$('#content-wrapper').css({'margin-top' : '100px'});
		$('#search-title').css({'top' : '-75px'});
	}else if(heightBlock >= 52 && heightBlock <=77){
		$('#content-wrapper').css({'margin-top' : '110px'});
		$('#search-title').css({'top' : '-85px'});
	}else if (heightBlock >= 78 && heightBlock <=103) {
		$('#content-wrapper').css({'margin-top' : '120px'});
		$('#search-title').css({'top' : '-95px'});
	}    else if (heightBlock >= 104 && heightBlock <=129) {
		$('#content-wrapper').css({'margin-top' : '130px'});
		$('#search-title').css({'top' : '-105px'});
	}else if (heightBlock >= 130 && heightBlock <=155) {
		$('#content-wrapper').css({'margin-top' : '170px'});
		$('#search-title').css({'top' : '-145px'});
	}else if (heightBlock >= 156) {
		$('#content-wrapper').css({'margin-top' : '190px'});
		$('#search-title').css({'top' : '-165px'});
	}
*/

	//Шкала дат, открытие годов

	$('#open_date').live('click',function(e){
		var triger = $('#popup_window').css('display');

		if(triger == 'none'){
			$('#popup_window').fadeIn(100);
		}else{
			$('#popup_window').fadeOut(100);
		}

			$(document).bind('click.myEvent touchend.myEvent', function(e) {
				if ($(e.target).closest($('#popup_window')).length == 0) {
					$('#popup_window').fadeOut(100);
					$(document).unbind('click.myEvent touchend.myEvent');
				}
			});

		e.preventDefault();

	});

	//Скролл банера юриспруденция
	$(window).scroll(function () {
			var pageWrapperHeight = $('#page-wrapper').height();


		if ($(this).scrollTop() > 550 && ($(this).scrollTop() < (pageWrapperHeight - 365))) {
				$('#jur_banner').fadeIn();

				$('#jur_banner').css({'position' : 'fixed'});
			} else {
				if($(this).scrollTop() < (pageWrapperHeight - 365)){
					$('#jur_banner').fadeOut();
				}
			}
		});



});


$(document).ready(function(){
	var offset = $(".fixedblock").offset();
	$(".fixedblock").appendTo($("#page-wrapper"));

	// содержание и поиск
	$(window).scroll(function(){
		if(!offset) return;
		if ($(window).scrollTop() > offset.top) {
			$(".fixedblock").css({'top': '0px', 'position': 'fixed'});
			$(".fixedblock_wrapper").css('margin-left','auto');
		} else {
			$(".fixedblock").css({'top': '0px', 'position': 'absolute'});
			$(".fixedblock_wrapper").css('margin-left','auto');
		}

		if (950 < $('body').width() < $('#page-wrapper').width() & $(this).scrollLeft() > 0) {
			$('.fixedblock').css({left: -($(this).scrollLeft())});
		} else {
			$('.fixedblock').css({left: 0});
		}
	});

});