/**
*	Класс для микроплатежей
*
*	@author Mark V. Lvov
*	@email lvov.web@gmail.com
*/

var micropays = (function () {
    
    return {
        
		//Инициализация
        init: function (document_id,doc_access) {
			
			this.interval = 5;
			
			this.document_id = document_id;
			this.doc_access = doc_access;
			
			//Проверка формы
			this.checkPayFormHandler();
			
			//Проверка платежа
			this.checkPaymentHandler();
			
            // Хак для ИЕ для отправки формы по enter
            this.ieLoginHukHandler();
			
			//Повтор платежа
			this.retryPaymentHandler();
			
			//Перегрузка способа авторизации
			this.overloadAuthMethods();
			
			//Хендлер загрузки приложенных файлов документа
			this.getAttacheHandler();
			
			//Хендлер кнопки купить из модального окна (открывается при клике на иконку скачать файл)
			this.fileModalBuyDocHandler();
			
			this.debug = false;
        },
        
		getAttacheHandler: function () {
		
			var _this = this;

			//Нет кнопки покупки - нет продажи			
			if($(".buy_document").length < 1){
				
				//Если есть кнопка демо, тогда разрешается заказать демо, иначе документ бесплатный
				if($(".order_demo").length > 0){
					$(document).on("click","a[href*='docattache']",function () {
						$.fancybox.open({
							type: 'html',
							content: '<div class="modalFromFile"><p><strong>Получить форму данного документа вы сможете после приобретения<br/> профессиональных справочных систем «Кодекс»/«Техэксперт».<br/> Закажите демонстрацию систем в вашем регионе.</strong></p><div class="modalWindowButton"><a id="demoFromFile" class="demoButton single order_doc" href="http://www.cntd.ru/zakaz_demonstracii_produkta.html?doc='+_this.document_id+'">Заказать демо</a></div></div>'
						});
						return false;
					});
				}

			}else{
				
				//Проверка на авторизацию
				var authin = $(".HelloName").length > 0 ? true : false;
				
				//Пользователь неавторизован и доступа нет
				if(!authin && _this.doc_access != 1){
					//Показываем форму авторизации
					//$("a[href*='docattache']").addClass("login").attr('href','#loginform');
					$(document).on("click","a[href*='docattache']",function () {
						$.fancybox.open({
							type: 'html',
							content: '<div class="modalFromFile">'
										+'<p><strong>Загрузить файл можно после покупки документа.</strong></p>'
										+'<div class="modalWindowButton">'
											+'<a id="buyFromFile" class="demoButton single" href="javascript:void(0);">Купить документ</a>'
										+'</div>'
										+'<p style="color:#808080; margin-top: 5px;">Купить документ можно без регистрации. После покупки, вам<br/> будет автоматически предоставлен доступ в личный кабинет.</p>'
										+'<div class="modalWindowButton">'
											+'<a class="demoButton single login" href="#loginform">Авторизоваться</a>'
										+'</div>'
										+'<p style="color:#808080; margin-top: 5px; margin-bottom: 15px;">Авторизуйтесь, если вы уже зарегистрированы. После покупки<br/> документ появится в вашем личном кабинете.</p>'
									+'</div>'
						});
						return false;
					});
				}
				
				//Пользователь авторизован и доступа нет
				if(authin && _this.doc_access != 1){
					$(document).on("click","a[href*='docattache']",function () {
						/**/
						$.fancybox.open({
							type: 'html',
							content: '<div class="modalFromFile">'
										+'<p><strong><nobr>Загрузить файл можно после покупки документа.</nobr></strong></p>'
										+'<div class="modalWindowButton">'
											+'<a id="buyFromFile" class="demoButton single" href="javascript:void(0);">Купить документ</a>'
										+'</div>'
									  +'</div>'
						});
						return false;
					});
				}

			}
		
		},
		
		fileModalBuyDocHandler: function () {
			
			var _this = this;
			
			$(document).on("click","#buyFromFile",function () {
				$.fancybox.close();
				$(".buy_document").click();
				$('html, body').animate({
					scrollTop: $(".partialAccess_wrapper").offset().top
				}, 500);
			});
			
			
		},
		
		checkPayFormHandler: function () {
			
			var _this = this;
			
			$("#payment_submit").click(function(e){
				
				e.preventDefault();
				
				_this.checkFrom();
				
				
			});
			
			
		},
				
		
		checkFrom: function () {
			
			var _this = this;
			
			if(_this.debug){console.log("checkFrom");}
			
			_this.clearErrors();
			
			var errors = {};
			
			var login = true;
			
			if($("#spUserDataId").val() == ""){
				login = false;
			}
			
			//Проверяем выбран способ оплаты или нет
			var pay_system = $("input[name='method']:checked").val();
			if(typeof pay_system == "undefined"){
				errors['payment'] = "empty";
			}
			
			
			if(!login){//Если неавторизован
								
				//Проверяем email, если пользователь не авторизован
				var mail = $("#mail").val();
				if(mail == ""){
					errors['mail'] = "empty";
				}
				
				//Код страны
				var phone_country = $("#phone_country").val();
				if(phone_country == ""){
					errors['phone_country'] = "empty";
				}
				
				//Проверяем код
				var code = $("#code").val();
				if(code == ""){
					errors['code'] = "empty";
				}
				
				//Проверяем телефон
				var number = $("#number").val();
				if(number == ""){
					errors['number'] = "empty";
				}
				
			}else{
				var mail = $("#spUserEmail").val();
				if(mail == ""){
					errors['mail'] = "empty";
				}
			}
			
			//Проверяем ошибки
			var success = _this.checkErrors(errors);
			
			if(success){

				//Проверяем email на занятость
				$.ajax({
					url: '/payment/init/',
					data:  {document_id: _this.document_id,
							email: mail, 
							phone_country: phone_country,
							code: code,
							number: number,
							method: pay_system
						},
                    cache: false,
					dataType: 'json',
					type: 'POST',
					beforeSend: function(){
						_this.preloader("show");
					},
					success: function (data,textStatus){

						_this.preloader("hide");

						if(_this.debug) {console.log("Response textStatus: "+textStatus+"("+data['status']+")")};

						switch(data['status']){

							case 'ok':

								if(_this.debug){console.log("Form submit Success");}

								// Делаем переадресацию на оплату
								window.location.href = data['params']['link'];


							break;

							case 'err':

								var err = {};

								if(login && data['msg'] == 'email_isreg'){
									err['out_of_auth'] = data['msg'];
								}else{
									err['create_payment'] = data['msg'];
								}

								_this.checkErrors(err);
								
								
								
								if(_this.debug){console.log("Form submit False");}

							break;

						}

					},
					error: function (XMLHttpRequest, textStatus, errorThrown) {

						_this.preloader("hide");

						if(_this.debug){
							console.log(XMLHttpRequest);
							console.log(textStatus);
							console.log(errorThrown);
						}

						//Сообщаем о непредвиденной ошибке
						var err = {};

						err['connect'] = "error";
						_this.checkErrors(err);

						if(_this.debug){console.log("Form submit False");}

					}
				});
				
			}	   
		   
			

		},
		
		/*
		 * Убирает вывод ошибок
		 */
		clearErrors: function (){
			
			var _this = this;
			
			if(_this.debug){console.log("clearErrors");}
			
			$(".pay_err").removeClass("pay_err");
			
			//Убираем блок с ошибками
			$(".err_message").css("display","none");
			$(".err_message").html("");
		},

        // Хак для ИЕ 8, который не хочет отправлять формы по нажатию ENTER;
        ieLoginHukHandler: function () {
        var _this = this;
        // Сабмит для формы где вводим телефон и метод оплаты

        $('#payform').on("keydown", function(event){
            if (($.browser.msie) && ($.browser.version == '8.0' || $.browser.version == '7.0')) {
                if (event.keyCode == 13)
                {
                    return _this.checkFrom();
                }
            }
        });
    },
		
		/*
		 *	Показывает прелоадер при проверке формы
		 */
		preloader: function (action) {
			
			var _this = this;
			
			if(_this.debug){console.log("Preloader: "+action);}
			
			switch(action){
				case "show":
					$("#payment_submit").removeClass('payment_submit').addClass('payment_submit_ld');
					break;
				case "hide":
					$("#payment_submit").removeClass('payment_submit_ld').addClass('payment_submit');
					break;
			}
			
			return;
			
		},
		
		/*
		 *	Вывод ошибок
		 */
		 checkErrors: function (errors) {
			 
			var _this = this;
			 
			if(_this.debug){console.log("checkErrors");}
			 
			var err_count = 0;
			for (e in errors) { err_count++; }
			
			if(err_count > 0){
				
				if(_this.debug){console.log(err_count+" errors found!");}
				
				var text = [];
				
				$.each(errors,function(key,value){
					switch(key){
						case "connect":
							text.push("Ошибка соединения, пожалуйста повторите позже.");
						break;
						case "payment":
							$("#pay_method_title").addClass("pay_err");
							text.push("Укажите метод оплаты.");
							break;
						case "mail":
							$("input[name='mail']").addClass("pay_err");
							switch(value){
								case "empty":
									text.push("Не указан email.");
									break;
							}
							break;
						case "phone_country":
							$("input[name='phone_country']").addClass("pay_err");
							text.push("Не указан телефонный код страны.");
							break;
						case "code":
							$("input[name='code']").addClass("pay_err");
							text.push("Не указан код телефона.");
							break;
						case "number":
							$("input[name='number']").addClass("pay_err");
							text.push("Некорректный номер телефона.");
							break;
						//Вывод ajax ошибок
						case "create_payment":
							if(_this.debug){ console.log("create_payment: "+ value); }
							switch(value){
								case "email_empty":
									$("input[name='mail']").addClass("pay_err");
									text.push("Не указан email.");
									break;
								case "email_invalid":
									$("input[name='mail']").addClass("pay_err");
									text.push("Некорректный e-mail.");
									break;
								case "email_isreg":
									$("input[name='mail']").addClass("pay_err");
									text.push('Указанный вами email уже зарегистрирован.<br/>'
												+'Если это ваш email, <a id="special_auth" class="login" href="#loginform"> пройдите авторизацию по этой ссылке</a>.');
									//Прописываем email
									$("#email").val($("#mail").val());
									break;
								case "user_noid":
									text.push("Ошибка идентфикации пользователя.");
									break;
								case "empty_number":
									text.push("Не указан телефон.");
									break;
								case "doc_noid":
									text.push("Ошибка идентфикации документа.");
									break;
							}
							break;
						case "out_of_auth":
								switch(value){
									case "email_isreg":
										$("input[name='mail']").addClass("pay_err");
										text.push('Вы не авторизованы, пожалуйста <a id="special_auth" class="login" href="#loginform"> пройдите авторизацию по этой ссылке</a>.<br/>');
										//Прописываем email
										$("#email").val($("#mail").val());
									break;
								}
							break;
					}
					 
				});		
				
				if(text.length > 0){
					//Отображаем ошибки
					var html = "";
					for(var i=0;i<text.length;i++){
						if(i == 0){
							html += text[i];
						}else{
							html += "<br/>"+text[i];
						}
					}
					//Выводим блок с ошибками
					$(".err_message").html(html);
					$(".err_message").css("display","block");
				}

				return false;
				
			}else{
				
				if(_this.debug){console.log("Errors not found");}
				return true;
				
			}

		 },
		 
		/*
		 * Вешаем проверку завершения платежа
		 */
		checkPaymentHandler: function () {
			
			var _this = this;
			
			//Возможно была совершена покупка
			if(_this.doc_access == 2 || _this.doc_access == 3){
				
				_this.checkingProcessed();
				
			}
			
		},
		
		/*
		 *	Отображает блок с информацией о завершении платежа
		 */
		showPaymentStatus: function (status) {
			
			var _this = this;
			
			if(_this.debug){console.log("showPaymentStatus");}
			
			if(status){//Платеж прошел успешно
				//Просто перезагружаем страницу
				//alert("Спасибо, Ваш платеж успешно завершен. Через 10 секунд страница будет перезагружена");
				//setTimeout(function(){
					window.location = window.location.href;
				//}, 10 * 1000);
			}else{//Платеж завершился неудачно
				//Меняем отображение
				//alert("Извините, но Ваш платеж завершился неудачно.");
				$("#doc_access_2").css("display","none");
				$("#doc_access_3").css("display","none");
				$("#doc_access_4").css("display","block");
				//Скролим в самый низ
				var footer = $(document).height();
				$('html,body').animate ({scrollTop: footer},1000);
			}
			
		},
		
		/**
		 *	Повтор платежа после неудачной попытки
		 */
		retryPaymentHandler: function () {
			
			var _this = this;
			
			$(".retry_payment,#button_try_again").click(function(){
				$("#doc_access_2").css("display","none");
				$("#doc_access_3").css("display","none");
				$("#doc_access_4").css("display","none");
				$(".payblock").css("display","block");
				return false;
			});
			
		},

		/**
		 *	Проверка завершения платежа
		 */
		checkingProcessed: function () {
			
			var _this = this;
						
			$.ajax({
				url: '/sprypay/paycheck/', 
				data:  {ids: _this.document_id},
				dataType: 'json',
				type: 'POST',
				beforeSend: function(){

				},
				success: function (json,textStatus){

					if(_this.debug) {console.log("Response textStatus: "+textStatus+"("+json['status']+")");}

					switch(json['status']){

						case 'ok':

							//Платеж завершен
							if(json['data'][_this.document_id]['processed'] === true){

								//Отображаем результат завершения платежа
								_this.showPaymentStatus(json['data'][_this.document_id]['status']);

							}else{//продолжаем проверять

								if(_this.debug){console.log("Continue checking...");}

								setTimeout(function(){

									_this.checkingProcessed();

								}, _this.interval * 1000);
							}

						break;

						case 'err':

							if(_this.debug){console.log("Some error checking...");}

							setTimeout(function(){
								_this.checkingProcessed();
							}, _this.interval * 1000);

						break;

					}

				},
				error: function (XMLHttpRequest, textStatus, errorThrown) {

					if(_this.debug){console.log("Fatal error checking...");}

					setTimeout(function(){
						_this.checkingProcessed();
					}, _this.interval * 1000);
				}
			});
			
		},
		
		//Читает параметры url
        getUrlParam: function (name) {

            var _this = this;
        
            var results = new RegExp('[\\?&]' + name + '=([^&#]*)').exec(top.window.location.href); 
            return (results !== null) ? results[1] : false;
 
        },
		
				
		/*
		 * Перегружает способ авторизации при нажатии на кнопку авторизации при покупке
		 * 
		 * Сразу после авторизации срабатывает форма покупки документа, которая формирует платеж и сабмитит на sprypay
		 */
		overloadAuthMethods: function () {
			
			var _this = this
			//Выключает дефолтный сабмит после авторизации и включает покупку после авторизации
			$("body").on("click", "#special_auth", function(e){
				authorization.buyWithAuthEnableHandler();
			});
			
		},
		
		//Заполняет форму платежа SpryPay для ранее неавторизованного пользователя (покупка с авторизацией)
		buyWithAuth: function (user_id,email) {
			
			var _this = this;

			//Заполняем форму
			$("#spUserDataId").val(user_id);
			$("#spUserDataEmail").val(email);
			$("#spUserEmail").val(email);
			//Отправляем данные для формирования платежа
			_this.checkFrom();
			
		}
		
        
    }
    
})();