/* 
 * Класс для работы с текстом документа
 * 
 */
Document.scroll = function(){

		loader_template_html = '<div class="cell_loader"><div class="child"><p><img src="/general/images/ajax-loader.gif" alt="" /><br />Загружается фрагмент</p></div><div class="helper"></div></div>';
		var options  = {
			 document_id : 0,

			URL : 'http://' + (typeof(HOST) != 'undefined' && HOST != '' ? HOST : 'http://'+HOST ) + '/',
		
			loader_template : $('<div/>').html(loader_template_html).contents(),
		
			error_template :  $('<div/>').html('<div class="err_cell_loader"><div class="child"><p><img src="/general/images/err_load.jpg" alt="" /></p><p><span>Не удалось загрузить фрагмент</span></p><p>Вероятнее всего, это вызвано временными техническими неполадками на сервере.<br />Пожалуйста, попробуйте повторить запрос позднее.</p></div><div class="helper"></div></div>').contents(),
			element_to_scroll : ''
		};

		function init_big(document_id, element_to_scroll, block) {
			options.document_id = document_id;
			options.element_to_scroll = element_to_scroll;
			options.block = block + 1;
			options.block_up = block - 1;
			scrollDown(options);
			if(options.block_up > 1) scrollUp(options);

			options.chapter_loading = false;

			//навешиваем обработчик на нажатие ссылки в оглавлении (перемотка)
			$('ul.soderjanie span a').click(function () {
				//Получаем хреф главы
				var current_chapter_id = $(this).attr('href');

				var id = current_chapter_id.replace('#', 'h_').replace('h_h_', 'h_');

				//если элемент есть на странице, скрол к нему
				if($('#'+id).length){
					$.scrollTo($('#'+id), 1500, {offset:-170} );
				}else{
					//сбрасываем все скролы
					$(element_to_scroll).autobrowse('flush');
//					options.loader_template = options.loader_template.html(loader_template_html).contents();
					getChapter(id);
				}
				return false;
			});
			$('#previous_block, #next_block').hide();
			
			//клик по кнопке наверх
			$('#back-top a').click(function(){
				//сбрасываем все скролы
				$(options.element_to_scroll).autobrowse('flush');
//				options.loader_template = options.loader_template.html(loader_template_html).contents();
				getBlock(1);
				return false;
			});
		}
		
		function init(){
			//навешиваем обработчик на нажатие ссылки в оглавлении (перемотка)
			$('ul.soderjanie span a').click(function () {
				//Получаем хреф главы
				var current_chapter_id = $(this).attr('href');

				var id = current_chapter_id.replace('#', 'h_').replace('h_h_', 'h_');

				//если элемент есть на странице, скрол к нему
				if($('#'+id).length){
					$.scrollTo($('#'+id), 1500, {offset:-170} );
				}
				return false;
			});
			
		}
		
		function scrollDown(options){
			$(options.element_to_scroll).autobrowse({
				url: function (offset) {
					return options.URL + 'api/document/get_block?document_id=' + options.document_id + '&block=' + (offset + 1);
				},
				before: function(response){
					var tables = $(options.element_to_scroll).find('table:last');
					tables.each(function(){
						if($(this).parent().is('#wrapperForTab') || $(this).parent().is('#justWrapper')){
							tables.unwrap();
						}
					});
				},
				template: function (response) {
					return response;
				},
				after: function(response){

					var tables = $(options.element_to_scroll+' > table');

					// Обрабатываем широкие таблицы
					Document.setTableScrolling(tables)	;

				},
				complete: function(obj, response, newElements){
					options.block += 1;
					$('#next_block a').attr('href', '/document/' + options.document_id + '?block=' + options.block)
				},
				itemsReturned: function (response) {
					return (response == null || response == '') ? 0 : 1;
				},
				sensitivity: 500,
				useCache: false,
				offset: options.block - 1,
				json: true,
				gluetable:true,
				loader: options.loader_template,
				finished: function () {
					$(this).append('<p class="end" style="text-align:center">Конец документа</p>');
					setTimeout(function() {$('.end').hide(500)}, 3000);
					setTimeout(function() {$('p.end').remove();}, 3100);
					$('#next_block').hide();
				},
				error_template: options.error_template
				
			});
		}
		
		function scrollUp(options){
			$(options.element_to_scroll).autobrowse({
				url: function (offset) {
					var offset2 = (offset + -1);
				
					return options.URL + 'api/document/get_block?document_id=' + options.document_id + '&block=' + offset2;
				},
				before: function(response){
					var tables = $(options.element_to_scroll).find('table:first');

					tables.each(function(){
						if($(this).parent().is('#wrapperForTab') || $(this).parent().is('#justWrapper')){
							tables.unwrap();
						}
					});
				},
				template: function (response) {
					return response;
				},
				after: function(response){

					var tables = $(options.element_to_scroll+' > table');

					// Обрабатываем широкие таблицы
					Document.setTableScrolling(tables)	;
				},
				complete: function(obj, response, newElements){
					options.block_up -= 1;
					$('#previous_block a').attr('href', '/document/' + options.document_id + '?block=' + options.block_up);

					if(options.block_up <= 0)
						$('#previous_block').hide();
				},
				itemsReturned: function (response) {
					return (response == null || response == '' ) ? 0 : 1;
				},
				sensitivity: 500,
				up : 1,
				useCache: false,
				gluetable:true,
				offsetUp: currentOffset,
				json: true,
				loader: options.loader_template,
				error_template: options.error_template
				
			});
		}
		
		function getChapter(chapter_id){
			//Показываем ожидалку
			if(options.chapter_loading) return;
			$(options.element_to_scroll).html(options.loader_template);
			$.scrollTo($(options.loader_template), 0, {offset:-170} );
			options.chapter_loading = true;
			Api.send(
				'api/document', 'getblockbychapterid',
				{ chapter_id: chapter_id,
				  document_id : options.document_id
				},
				function (response) {
					if(response == null || response ==''){
						$(options.element_to_scroll).html(options.error_template);
						$.scrollTo($(options.error_template), 0, {offset:-170} );
						return;
					}
					response = $.parseJSON(response);
					//пришел весь документ
					if (response.all == 1) {
						$(options.element_to_scroll).html(response.html);
						$(options.element_to_scroll).autobrowse('flush');
						var id = chapter_id.replace('#', 'h_').replace('h_h_', 'h_');
						$.scrollTo($('#'+id), 0, {offset:-200} );
					}else{
						$(options.element_to_scroll).html(response.html);
						//Скролл к главе
						var id = chapter_id.replace('#', 'h_').replace('h_h_', 'h_');
						$.scrollTo($('#'+id), 0, {offset:-200} );

						// Установка лоалера заново, фикс бага в ИЕ, когда лоадер пропадал
						options.loader_template = $('<div/>').html(loader_template_html).contents();

						currentOffset = response.order;

						setTimeout(function(){scrollDown(options)}, 500);
						setTimeout(function(){scrollUp(options)}, 500);
					}
					//Обрабатываем таблицы
					Document.setTableScrolling($(options.element_to_scroll).find('table'));
					options.chapter_loading = false;
				},
				function () {
					$(options.element_to_scroll).html(options.error_template);
					$.scrollTo($(options.error_template), 0, {offset:-170} );
					options.chapter_loading = false;
					return;
				}
			);
		}
		
		function getBlock(block){
			//Показываем ожидалку
			if(options.chapter_loading) return;
			$(options.element_to_scroll).html(options.loader_template);
			$.scrollTo($(options.loader_template), 0, {offset:-170} );
			options.chapter_loading = true;
			Api.send(
				'api/document', 'get_block',
				{ block: block,
				  document_id : options.document_id
				},
				function (response) {
					if(response == null || response ==''){
						$(options.element_to_scroll).html(options.error_template);
						$.scrollTo($(options.error_template), 0, {offset:-170} );
						return;
					}
					response = $.parseJSON(response);
					//пришел весь документ
					if (response.all == 1) {
						$(options.element_to_scroll).html(response.html);
						$(options.element_to_scroll).autobrowse('flush');
					}else{
						$(options.element_to_scroll).html(response.html);

						// Установка лоалера заново, фикс бага в ИЕ, когда лоадер пропадал
						options.loader_template = $('<div/>').html(loader_template_html).contents();

						currentOffset = block;

//						setTimeout(function(){scrollDown(options)}, 500);
						scrollDown(options);
					}
					//Обрабатываем таблицы
					Document.setTableScrolling($(options.element_to_scroll).find('table'));
					options.chapter_loading = false;
				},
				function () {
					$(options.element_to_scroll).html(options.error_template);
					$.scrollTo($(options.error_template), 0, {offset:-170} );
					options.chapter_loading = false;
					return;
				}
			);
		}
		
		
		return {
			init : init,
			init_big: init_big
		}
}();
