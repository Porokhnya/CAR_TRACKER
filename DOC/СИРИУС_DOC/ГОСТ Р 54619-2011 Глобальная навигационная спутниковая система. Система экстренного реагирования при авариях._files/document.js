/* 
 * Класс для работы с текстом документа
 * 
 */
Document = function ($) {
	return {
		
		init: function () {
			
			// Приделываем горизонтальный скроллинг к таблицам и широким картинкам
			this.setTableScrolling($('#doc_with_soderjanie').find('table'));
			//this.setImgScrolling($('#doc_with_soderjanie').find('img'));
			
			$('#st_close_btn').unbind().click(
				function () {
					$('#index_string_wrapper').hide(500);
				}
			);
	
			this.pdfHandler();

		},

		// Ожидаем оплату документа, после перагрузим страницу
		waitPayment: function (id) {
			var interval = setInterval(function() {
				$.getJSON( "/payment/paid-state?document_id=" + id, function( json ) {
					// Оплата поступила - перезагружаем
					if(json.paid) {
						clearInterval(interval);
						location.reload();
					}
				});
			}, 3000);

		},
		
		pdfHandler: function () {
		
			$("#click_on_pdf").click(function(){
				$(".buttonPDF_download").click();
			});
			
		},
		
		
		// Добавляет горизонтальный скроллинг к широким элементам в документе
		setTableScrolling: function (element) {
			
			// Добавляет скроллинг таблицам для доков с содержанием
			$(element).each(
				function () {
					if($(this).width() == 0){
						var overflow = true;						
					}else{
						var overflow = false;
					}
					if($(this).parent().not('#wrapperForTab') || $(this).parent().not('#justWrapper')){
                        if (($(this).width()+30 > $('#doc_with_soderjanie').width()) || overflow) {
							$(this).wrap('<div id="wrapperForTab"></div>');
//							console.log($(this).width() - $('#doc_with_soderjanie').width())
							var maxRight = $(this).width() - $('#doc_with_soderjanie').width()
							$(this).draggable({ cursor: "move", axis: 'x'});
						}else{
							//$(this).wrap('<div id="justWrapper" style="padding: 0px 1px; width: 650px; margin:0 45px 0 70px;" />'); Это было зачем?? я про отступ в стилях OLEG
							$(this).wrap('<div id="justWrapper" style="padding: 0px 1px;" /></div>');
                            $(this).css({margin:'0 auto'});
						}
					}
				}
			);		

		},
				
		setImgScrolling: function (element) {
			
			// Скролинг для широких картинок
			$(element).each(
				function () {		
					if ($(this).width()+70 > $('#doc_with_soderjanie').width()) {
					  $(this).wrap('<div style="overflow-x:auto;overflow-y:hidden; padding: 0px 1px; width: 650px;"></div>');						
					}
				}
			);				
		},
		
		
		/*setDjvuNavigate: function (block_id) {
			
			var _this = this;
			
			//Инициализация
			var tabs = $('#tabs-selector-low > div.current-loww');
			var left = [];
			$.each(tabs, function(i, val){
				var width = $(val).outerWidth(true) / 2 ;
				if(i != 0){
					left[i] = {'id' : val.id ,'width2': parseInt(width), 'width' : parseInt(width + left[(i-1)].width2+ left[(i-1)].width)};
				}else{
					left[i] = {'id' : val.id , 'width2' : parseInt(width), 'width' : parseInt(width)};
				}
			});

			$('#switch-tab').css('left', left[0].width);	

			if (typeof block_id != 'undefined') {
				_this.moveTreangle(left, block_id);
			} else {
				//Перемещение ползунка на вкладках текст и статус
				$('#tabs-selector-low > div').live('click',
					function() {
						_this.moveTreangle(left, this.id);
					}
				);
			}
			
		},
		
		moveTreangle: function (left, block_id) {			
			$.each(left, function(i, val){
				
				if(val.id == block_id){
					var trololo = parseInt(val.width);
					if( $.browser.opera ){
						 $('#switch-tab').css('left', trololo);
					}else{							     
						$('#switch-tab').animate({ 
							left:trololo
						}, 500);
					}							
				}
			});
		}*/
	}	
} (jQuery);