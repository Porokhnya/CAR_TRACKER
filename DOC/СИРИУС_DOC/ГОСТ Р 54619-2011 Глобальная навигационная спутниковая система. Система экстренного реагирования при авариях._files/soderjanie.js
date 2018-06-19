/**
*	Класс для управления страницами сканеркопий
*
*	@author Sergey S. Sergienko
*	@email mr.debugg@gmail.com
*/

Soderjanie = function ($) {
	
	return {

		/*
		 * Навешивает он клик на все ссылки в оглавлении
		 */
		init: function (document_id) {

			// Навешиваем обработчик на кнопку открытия и закрытия содержания
			this.setSoderjanieControlHeandlers();
			
			// Навешиваем обработчики на прокручивание документа, чтобы содержание всенда слева болталось
			this.setTocScrollHeandlers();
			
			// Навешиваем обработчики на стрелки раскрывающие и закрывающие категории меню
			this.setCatHeandlers();
			
		},
		
		// Обработчики на стрелки раскрывающие и закрывающие категории меню
		setCatHeandlers: function () {
		
			// Закрываем категорию
			$('.ar_opened').live('click', function() {				
				$(this).removeClass('ar_opened').addClass('ar_closed');
				$(this).parent().children('ul').hide(300);
			});
			
			// Открываем категорию
			$('.ar_closed').live('click', function() {
				$(this).removeClass('ar_closed').addClass('ar_opened');
				$(this).parent().children('ul').show(300);
			});
		},
		
	
		
		showError: function (text) {
            $('#doc-loading').html(text);
		},
                
		/*
		 * Скрывает содержание
		 **/
		hideSoderjanie: function () {
            $('#content_field').fadeOut(300);
		},

		/*
		 * Показывает содержание
		 **/
		showSoderjanie: function () {
            $('#content_field').fadeIn(300);
			$("#search_field .search_form").focus();
		},
		
		/*
		 * Открывает форму поиска
		 **/
		showSeacher: function () {
			$('#search_window').fadeIn(300);
			$('#content_field').fadeOut(300);
			$("#search_field .search_form2").focus();
		},

		/*
		 * Закрывает форму поиска
		 **/
		hideSeacher: function () {
			$('#search_window').fadeOut(300);
			$('#doc_with_soderjanie span.searchText').contents().unwrap();
		},
		
		/*
		 * При пролистывании страницы вниз содержание остается на месте
		 **/
		setTocScrollHeandlers: function () {
         /*$('#search_and_content').sticky({topSpacing:30});*/
		/* $('.search_and_content_block').sticky({topSpacing:30});*/
			/*$('.content_field').sticky({topSpacing:30});*/
            /*$('#bannerCloseDoc').sticky({topSpacing:0, minHeight: 76, maxHeight: 142});*/

		},
		
		/*
		 * Навешивает обработчик на кнопку открытия и закрытия содержания
		 **/
		setSoderjanieControlHeandlers: function () {

			var _this = this;

			// Показать содержание
			$('#content').unbind().click(function () {


				_this.showSoderjanie();
			});

			// Скрыть содержание
			$('#content_field #content_header .close').click(function () {
				_this.hideSoderjanie();
			});

			//Закрыть поиск
			$('#search_window #content_header .close').live('click',function() {
				_this.hideSeacher();				
			});

			//Открыть поиск
			$('#search').live('click',function() {
				_this.showSeacher();
								
			});
		}
	}
	
}(jQuery);