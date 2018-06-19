/**
 * Класс api
 *
 * @author ssergienko@kodeks.ru
 * 
 **/
Api = function () {
	
	return {
		
		// URL для доступа к API
		URL: 'http://' + (typeof(HOST) != 'undefined' && HOST != '' ? HOST : 'http://'+HOST ) + '/',
		
		// Отправляет запрос контроллеру и экшону, указанным в параметрах и возвращает результат
		send: function (controller, action, data, successCallback, errorCallback, httpErrorCallback) {
			
			// Проверяем данные
			if( !controller ) {
				alert('Controller is not defined');
			}
			if( !action ) {
				alert('Action is not defined');
			}
			
			// Если данные не указаны, объявляем пустой объект
			if( !data ) {
				data = {};
			}

			// Если очистка кеша, увеличиваем таймаут
			var timeout = 0;
			var type = 'html'
			if (controller == 'jsapi/clearcache' || controller == 'api/chapter') {
				timeout = 60000;
				type = 'json'
			} else {
				timeout = 30000;
				type = 'html'
			}

			// Готовим URL
			var url = this.URL + controller + '/' + action;
			
			// Замыкание
			var _this = this;
			
			// Выполняем запрос и возвращаем объект запроса
			return $.ajax({
	
				// Метод (GET обязателен)
				type: "GET",
				
				// Запрещаем кеширование
				cache: false,

				// Таймаут
				timeout: timeout,

				// Формат ответа (JSONP обязателен)
				dataType: type,

				// Адрес прокси
				url: url,

				// Данные для прокси
				data: data,

				// Успешный ответ от прокси
				success: function(response) {
					// Если на API была ошибка, вызываем обработчик ошибки
					if(typeof(response) == 'undefined' || response == '') {

							// Если существует обработчик ошибки, вызываем его
							if(errorCallback) {

								errorCallback(response);

							} else {

								// Обработчик не указан, поэтому вызываем дефолтный обработчик
								_this.defaultFailureCallback(response);

							}

					} else {

						// Ошибки не было

						// Если существует обработчик успешного результата, вызываем его
						if(successCallback) {

							successCallback(response);

						} else {

							// Обработчик не указан, поэтому вызываем дефолтный обработчик
							_this.defaultSuccessCallback(response);

						}

					}

				},
	
				// Ошибка прокси
				error: function(xhr, textStatus, errorThrown) {

					// Если есть обработчик таких ошибок, обрабатываем
					if(errorCallback){
						errorCallback();
					}else if(httpErrorCallback) {

						httpErrorCallback(xhr, textStatus, errorThrown);

					} else {

						// Сообщаем, что ошибка соединения
						alert('Temporary connection error');

					}

				}

				
			});
		
		},

		/**
		 * Дефолтный обработчик ошибки API (не прокси)
		 * @param response:object JSON-объект, пришедший от API
		 */
		defaultFailureCallback: function (response) {

			//intive.hideLoadScreen(this);
			
			// Сообщаем об ошибке
			//alert(response.error);

		},


		/**
		 * Дефолтный обработчик успешного ответа API (не прокси)
		 * @param response:object JSON-объект, пришедший от API
		 */
		defaultSuccessCallback: function (response) {

			// Ничего не делаем

		}
	
	}
	
}(jQuery);