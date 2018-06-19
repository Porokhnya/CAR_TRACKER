/* 
 * Главный объект для интерфейса. Тут нужно писать всякие методы которые используются на любых страницах сайта.
 * 
 */

var General = (function () {	
	return {


		// Затемняет экран
		showShadow: function () {		
 
			$('.cell_loader').remove();
			var el = '<div class="cell_loader"><div class="child"><p><img src="/general/images/ajax-loader.gif" alt="" /><br />Загружается глава</p></div><div class="helper"></div></div>';
			
			
		//	$(el).css('margin-top' , height);
			$('#doc_with_soderjanie').html(el);
			var height = $('.cell_loader').outerHeight(true);
			$('.cell_loader').css('margin-top' , height);
			$.scrollTo($('.cell_loader'));

		},
			
		// Убираем затемнение
		hideShadow: function () {
			$('.cell_loader').fadeOut();
			$('.cell_loader').remove();
		}


	}
	
})();