//Инициализация
$(document).ready(function() {
	authorization.init();
});

var authorization = (function () {

    return {

        init: function () {
			
			//Переменная указывает был ли уже сабмит формы
			this.submit_login = false;
			
			//Подключение фэнсибоксов
			this.fancyboxHandler();

			//Восстановление пароля
			this.passwordRecoveryHandler();
			
			//Подключение повторной отправки кода активации
			this.smsResendHandler();
			
			//Подключение сабмита формы авторизации по нажатию кнопки
			this.loginSubmitHandler();

			//Подключение хака для глючного ie
			this.ieLoginHukHandler();
			
			//скрытие ошибок при фокусе
			this.focusInput();
			
			//Очистка хэша после открытия формы авторизации
			this.hashReader();
			
			//хендлер обработки формы регистрации
			this.initRegFormHandler();
			
        },
		
		//Фансибоксы для авторизации/регистрации/восст.пароля
		fancyboxHandler: function () {
			
			var _this = this;
			
			//авторизация
			$(".login").fancybox({
				maxWidth	: 470,
				maxHeight	: 600,
				padding		: 0,
				fitToView	: false,
				width		: '70%',
				height		: '75%',
				'scrolling' : 'no',
				autoSize	: true,
				closeClick	: false,
				openEffect	: 'fade',
				//href		: '/user/loginform',
				//type		: 'ajax',
				helpers : {
					title:  null
				},
				closeEffect	: 'fade',
				beforeShow : function() {
                    this.title = null;
                    $('.fancybox-inner').addClass('fancybox-inner2');
				},
				afterShow : function() {
					//$("input[name=return_url]").val($(location).attr('pathname'));			
					$('#loginform #email').focus();
				},
				afterClose: function() {
					//По закрытию включает дефолтный сабмит после авторизации
					_this.loginSubmitHandler();
				},
				helpers : {
					title : null            
				}    
			});
			
			//Регистрация
			$(".reg").fancybox({
				maxWidth	: 540,
				padding		: 0,
				fitToView	: false,
				width		: '70%',
				height		: '75%',
				autoSize	: true,
				closeClick	: false,
				openEffect	: 'fade',
				'scrolling' : 'no',
				href		: '/user/regform',
				type		: 'ajax',
				closeEffect	: 'fade',
				beforeShow : function() {
                    this.title = null;
				$('.fancybox-inner').addClass('fancybox-inner2');

				},
				afterShow : function() {
					$('#loginform #email').focus();
					$('#reg-dialog input[name="email"]').focus();
				}
			});
			
			//Восстановление пароля
			$(".passrecovery").fancybox({
				maxWidth	: 470,
				maxHeight	: 600,
				padding		: 0,
				fitToView	: false,
				width		: '70%',
				height		: '70%',
				'scrolling' : 'no',
				autoSize	: true,
				closeClick	: false,
				openEffect	: 'fade',
				helpers : {
					title:  null
				},
				closeEffect	: 'fade',
				beforeShow : function() {
                    this.title = null;
                    $('.fancybox-inner').addClass('fancybox-inner2');
				},
				afterShow : function() {
					$('#passrecovery input[name="email"]').focus();
				}
			});

		},
				
		passwordRecoveryHandler: function () {
			
			var _this = this;
			
			$('#passrecovery').submit(function() {
				
				Errors.hide('#passrecovery');

				var data = {
					email: _this.trim($('#passrecovery input[name=email]').val())
				};

				// Проверяем e-mail
				var reg = /^([A-Za-z0-9_\-\.])+\@([A-Za-z0-9_\-\.])+\.([A-Za-z]{2,4})$/;

				if(reg.test(data.email) == false) {
					if(Errors.add('email','Некорректный e-mail').view('#passrecovery')){
						return false;
					}
				}

				$('#preloader').fadeIn();
				
				$.ajax({
					data: data,
					url: '/user/passrecovery',
					dataType: 'json',

					success: function(response) {	

						$('#preloader').fadeOut();

						// Если успешн
						if(response && response.result == true) {
							$('.reminder-message').fadeIn();
							$('.reminder-form').hide();
						}else{
							// Пишем ошибку
							Errors.add('email', response.message).view('#passrecovery');
						}
					}
				});

				$('#preloader').fadeOut();
				
				return false;
				
			});
			
		},
		
		smsResendHandler: function () {

			var _this = this;

			$('.sms_resend_button').live('click', function(){

				return _this.smsResend(this);

			});


		},
		
		smsResend: function (form) {

			var _this = this;
			
			var $this = $(form);
			
			Errors.hide('#form-register');

			var phone_code = $('#form-register input[name=phone_code]').val().replace(/\s+/g,'');
			var phone_country = $('#form-register input[name=phone_number]').val().replace(/\s+/g,'');
			var phone = phone_code && phone_country ? '7' + phone_code + phone_country : '';
			var data = {
				email: _this.trim($('#form-register input[name=email]').val()),
				phone: phone
			};

			$.ajax({

				data: data,
				url: '/user/resendsms',
				cache: false,
				dataType: 'json',

				success: function(response) {
					if(response.result){
						$('#sms_field').show(100);
						$this.hide(100);
						Errors.add('sms','Введите код из смс сообщения').view('#form-register');
					}else{
                        switch (response.message){
                            case 'Проверьте корректность заполненных полей':
                                Errors.add('sms', 'Поля "E-mail" и "Телефон" должны совпадать с теми, что вы указали в начале регистрации.').view('#form-register');
                                break;
                            default:
                                Errors.add('sms', 'Не удалось отправить смс, попробуйте еще раз').view('#form-register');
                        }
					}
				}
			});
			
			return false;
		
		},
		
		//хак для ИЕ 8 ибо он не хочет отправлять форму по нажатию enter сам, ленивая скотина
		ieLoginHukHandler: function () {
			
			var _this = this;

	        // Сабмитим с покупкой
          /*  $("body").on("click", "#special_auth", function(e){
                $('#login ').live("keydown", function(event){
                    if (($.browser.msie) && ($.browser.version == '8.0')) {
                        if (event.keyCode == 13)
                        {
                            buyWithAuth = true;
                            return _this.formSubmit(buyWithAuth);
                        }
                    }
                });
            });*/
            // Сабмит с покупкой
            $("body").on("click", "#special_auth", function(e){

                $('#login #email').on("keydown", function(event){
                    if (($.browser.msie) && ($.browser.version == '8.0')) {
                        if (event.keyCode == 13)
                        {
                            buyWithAuth = true;
                            return _this.formSubmit(buyWithAuth);
                        }
                    }
                });

                $('#login #password').on("keydown", function(event){
                    if (($.browser.msie) && ($.browser.version == '8.0')) {
                        if (event.keyCode == 13)
                        {
                            buyWithAuth = true;
                            return _this.formSubmit(buyWithAuth);
                        }
                    }
                });
            });

            // Сабмитим без покупки
            $('.login').click(function(){
                $('#login #email').unbind();
                $('#login #password').unbind();
                $('#login').on("keydown", function(event){
                    if (($.browser.msie) && ($.browser.version == '8.0')) {
                        if (event.keyCode == 13)
                        {
                            buyWithAuth = false;
                            return _this.formSubmit(buyWithAuth);
                        }
                    }
                });
            });
// Сабмит для формы где вводим телефон и метод оплаты
           /* $('#mail').on("keydown", function(event){
                if (($.browser.msie) && ($.browser.version == '8.0')) {
                    if (event.keyCode == 13)
                    {
                        buyWithAuth = false;
                        return _this.formSubmit(buyWithAuth);
                    }
                }
            });*/
            /*var buyWithAuth = false;
            $('.body').click(function(e){
                var target = $(e.target) || $(e.srcElement);
                console.log(target);
                console.log(target.attr('id'));
            });*/
			/*
			// хак для ИЕ 8 ибо он не хочет отправлять форму по нажатию enter сам. ((
			$('#login').live("keydown", function(event){
				if (($.browser.msie) && ($.browser.version == '8.0')) {
					
					if (event.keyCode == 13)
					{
							return _this.formSubmit();
					}
				}
			});
			 */
		},
		
		//Включает дефолтный сабмит после авторизации
		loginSubmitHandler: function () {
			var _this = this;
			var buyWithAuth = false;
			$('#login').unbind().bind("submit",function() {
				return _this.formSubmit(buyWithAuth);
			});
			
		},
		
		//Выключает дефолтный сабмит после авторизации и включает покупку после авторизации
		buyWithAuthEnableHandler: function () {

			var _this = this;
			var buyWithAuth = true;
			$('#login').unbind().bind("submit",function() {
				return _this.formSubmit(buyWithAuth);
			});
			
		},
		
		/*
		*	Сабмит формы 
		*	submit_after_auth (bool) необходимость сабмита формы после авторизации
		*/
		formSubmit: function (buyWithAuth) {
			var _this = this;
			
			//Повторный сабмит формы
			if(_this.submit_login){
				return true;	
			}
			Errors.hide('#login');

			var data = {
				email: _this.trim($('#login input[name=email]').val()),
				password :  $('#login input[name=password]').val(),
				return_url : $('#login input[name=return_url]').val(),
				search_url : $('#login input[name=search_url]').val()
			};

			// Проверяем e-mail
			var reg = /^([A-Za-z0-9_\-\.])+\@([A-Za-z0-9_\-\.])+\.([A-Za-z]{2,4})$/;

			if(reg.test(data.email) == false) {
				if(Errors.add('email','Некорректный e-mail').view('#login')){
					return false;
				}
			}

			if(data.password.length < 2){
				if(Errors.add('pass','Введите пароль').view('#login')){
					return false;
				}
			}
			$.ajax({
				data: data,
				url: '/user/login',
				cache: false,
				dataType: 'json',
				success: function(response) {
					// Если успешно
					if(response && response.result == true) {
						if(buyWithAuth){//Покупка после авторизации
							micropays.buyWithAuth(response.user_id,data.email);
						}else{
							_this.submit_login = true;
							$('#login').submit();
						}
						
						//window.location.reload(true);
					}else{
						if(response.message == 'notactive'){
								window.location.href = 'user/login/hash/'+response.hash+'/email/'+data.email;
						}else{
							// Пишем ошибку
							if(response.message.name != undefined){
								Errors.add(response.message.name,response.message.text).view('#login');
							}else{
								Errors.add('email',response.message).view('#login');
							}
						}
					}
				},
				error: function(){
					window.location.href = 'user/login/email/'+data.email;
				}
			});

			return false;
			
		},
		
		//При попытке войти в личный кабинет без авторизации выкидывает форму авторизации и удаляет хэш
		hashReader: function () {
			
			var _this = this;
			
			var hash = window.location.hash.substring(1);
			
			switch(hash){
				case "login":
					$("#login input[name='return_url']").val("/user/office");
					$("a[href='#loginform']").click();
					window.location.hash = "";
					//history.pushState('', document.title, window.location.pathname);
					break;
				case "subscribeconfirm":
					//Подписка подтверждена
					$.fancybox.open({
						type: 'html',
						content: '<div class="doneReg" style="display:block; text-align:center"><div class="done"></div><p><strong>Спасибо, Ваша подписка подтверждена!</strong></p></div>'
					});
					break;
				case "subscribefail":
					$.fancybox.open({
						type: 'html',
						content: '<div class="doneReg" style="display:block; text-align:center"><div class="sad"></div><p><strong>Подписка не активирована!</strong><br/><br/> Попробуйте отправить письмо потверждения повторно.</p></div>'
					});
					break;
			}
			
		},
		
		//Вспомогательная функция
		trim: function (str, charlist) {
			
			var _this = this;
			
			// IE 8 ругается на конструкции типа email: $('#passrecovery input[name=email]').val().trim() и дает ошибку поэтому пользуем эту функцию.
			charlist = !charlist ? ' \\s\\xA0' : charlist.replace(/([\[\]\(\)\.\?\/\*\{\}\+\$\^\:])/g, '\$1');
			var re = new RegExp('^[' + charlist + ']+|[' + charlist + ']+$', 'g');
			return str.replace(re, '');
			
		},
		
		//скрытие ошибок при фокусе поля ввода формы
		focusInput: function(){
			
			var _this = this;
		
			$('.forma').on('focus', 'input[type="text"]', function(){
				
				Errors.hide('#'+$(this).closest('.fancybox-inner').children('div').attr('id'));
			});
		},
		
		
		/*	Регистрация пользователя	*/
		
		initRegFormHandler: function () {
			
			var _this = this;
			
			//Подрубаем куфон
			Cufon.replace(".doneReg .subsButton, .doneReg .demoButton, .reg_submit");
			
			//Фокус на форме
			$('.forma').on('focus', 'input[type="text"]', function(){
				Errors.hide('#'+$(this).closest('.fancybox-inner').children('div').attr('id'));
              // Добавляем ФИО в текст про персональные данные   
                    $('#form-register input[name=fio]').change(function() {
                      var fullname = $(this).val();
                      $('.personalData_name b').remove();
                       if (fullname != '') {                       
                          $('.personalData_name').append('<b>, '+fullname+',</b>');                        
                       }
                    });
			});
			
			
			//инициализация обработки формы
			_this.registrateHandler();
			
			//Инициализация кнопок формы после окончания регистрации
			_this.compliteRegButtonsHandler();
			
			
		},
				
		//Отправка формы
		registrateHandler: function () {
			
			var _this = this;
			
			$(document).on('submit', '#form-register', function(){
				
				//Проверяем форму
				var data = _this.checkRegForm();
				
				//Если получены данные, то все ок
				if(data){
					//Если все ок, то отправляем запрос на регистрирацию
					_this.registrateUser(data);
				}
				
				return false;
				
			});
			
		},
		
		//Валидация формы на фронте
		checkRegForm: function () {
			
			var _this = this;
			
			Errors.hide('#form-register');
			
			//Проверяем sms скрыто поле или нет.
			var displaySms = $('#sms_field').css('display');

			if(displaySms != 'none'){
				var lenght = $('#form-register input[name=cmckod]').val().length;

				if(lenght < 4){
					if(Errors.add('sms','Код подтверждения меньше 4 цифр').view('#form-register')){
						return false;
					}
				}
			}

			//Регион
			var region = $('#form-register .region option:selected').val()
			var other = false;
			if(region == 'other'){
				var region = $('#form-register #drugoi').val()
				other = true;
			}
			
			// Collect data
			var data = {
				email: $('#form-register input[name=email]').val(),
				password: $('#form-register input[name=password]').val(),
				fullname: $('#form-register input[name=fio]').val(),
				phone_country: '+7',
				phone_code: $('#form-register input[name=phone_code]').val().replace(/\s+/g,''),
				phone_number: $('#form-register input[name=phone_number]').val().replace(/\s+/g,''),
                personal_data: $('#form-register input[name=personalData]').prop("checked"),
				type: $('#form-register input[name=type]:checked').val(),
				tariff: $('#form-register input[name=tariff]:checked').val(),
				region: region,
				activity: $('#form-register #activityReg option:selected').val(),
				cmckod: $('#form-register input[name=cmckod]').val(),
				other : other
			};

			//Тесты
			// Проверяем e-mail
			var reg = /^([A-Za-z0-9_\-\.])+\@([A-Za-z0-9_\-\.])+\.([A-Za-z]{2,4})$/;

			if(reg.test(data.email) == false) {
				Errors.add('email','Некорректный&nbsp;e-mail');
			}

			if(region.length == ''){
				Errors.add('region','Выберите регион');
			}
			
			if(data.activity.length == ''){
				Errors.add('activity','Выберите деятельность');
			}

			// Проверяем пароль
			if(data.password.length < 6) {
				Errors.add('pass','Пароль меньше 6 символов');
			}

			// Проверяем имя
			if(data.fullname.length < 5) {
				Errors.add('name','Пожалуйста,&nbsp;укажите&nbsp;ФИО');
			}

			// Проверяем телефон
			var reg = /^([0-9]+)$/;

			if(data.phone_country == '' || reg.test(data.phone_code) == false || reg.test(data.phone_number) == false ) {
				Errors.add('phone','Некорректный&nbsp;номер&nbsp;телефона');
			}
            
            // Проверяем есть ли галочка
            if (data.personal_data == false) {
                Errors.add('personal','Чтобы зарегистрироваться, нужно принять наши Условия использования');
            }

			//Если были ошибки, выходим
			if(Errors.view('#form-register')){
				return false;
			}
			
			return data;
			
		},
		
		//регистрация пользователя
		registrateUser: function (data) {
			
			var _this = this;
			
			$('#loader').fadeIn();

			$.ajax({

				data: data,
				url: '/user/register',
				cache: false,
				dataType: 'json',

				success: function(response) {
					
					$('#loader').fadeOut();
					
					// Если успешно, перезагружаем экран
					if(response && response.result == true) {
						
						//Подгрузка блока заказа демонстрации
						$.get("/user/regdone", {}, function(html){
							$.fancybox.close();
							_this.openFancybox(html);
						});
											
					}else{
						if(response.message == 'gosms'){
							$('#sms_field').show(100);
							Errors.add('sms','Введите код из смс сообщения').view('#form-register');
						}else if(response.message == 'Не удалось отправить смс, попробуйте еще раз'){
							Errors.add('sms',response.message).view('#form-register');
							$('#sms_resend').show(100);
							$('#sms_field').show(100);
						}else{
							// Пишем ошибку
							if(response.message.name != undefined){
								Errors.add(response.message.name,response.message.text).view('#form-register');
							}else{
								Errors.add('sms',response.message).view('#form-register');
							}
						}
					}
				},
				error: function () {
					$('#loader').fadeOut();
                }
				
			});

			return false;
			
		},
			   
		//Хендлер кнопок управления после подписки
		compliteRegButtonsHandler: function () {
		
			var _this = this;
			
			//Кнопка для открытия окна управления подпиской
			$(document).on("click","#rubricListLoad",function(){
				
				//Инициализируем модуля подписки
				subscribe.init({rubrics: [], 
								//Успешная загрузка списка рубрик
								successFunction: function(html) {
									$.fancybox.close();
									_this.openFancybox(html);
								},
								//закрытие списка рубрик
								closeFunction: function(){
									$.fancybox.close();
									$.get("/user/regdone", {}, function(html){
										_this.openFancybox(html);
									});
								}
							});
				
				return false;
			});

			//Кнопка для открытия окна формы заказа демонстрации
			$(document).on("click","#formOrderLoad",function(){
				
				//Инициализируем модуль заказа демонстрации
				orderdemo.init({successFunction: function(html){
									$.fancybox.close();

									_this.openFancybox(html);
								},
								closeFunction: function(){
									$.fancybox.close();
									$.get("/user/regdone", {}, function(html){
										_this.openFancybox(html);
									});
								}
							});
				
				return false;
				
			});
			

		
		},
		
		openFancybox: function (html) {
			
			var _this = this;
			
			$.fancybox.open({
				type: 'html',
				content: html,
				maxWidth	: 540,
				maxHeight	: 600,
				padding		:0,
				fitToView	: false,
				width		: '70%',
				height		: '70%',
				'scrolling' : 'no',
				autoSize	: true,
				closeClick	: false,
				openEffect	: 'fade',
				helpers : {
					title:  null
				},
				closeEffect	: 'fade',
				beforeShow : function() {
                    //$('.fancybox-inner').addClass('fancybox-inner2');
				}
			});
			
		}
		
				

	}
    
})();

//Плагин-хелпер для работы с ошибками
Errors = function ($) {
	return {
		errors : [],

		add : function (name, text){
			//шейк окна
			this.errors.push('<div id="err-'+name+'" class="err-message"><span class="arr"></span><div><nobr>'+text+'</nobr></div></div>');
			return this;
		},
		view : function (id){
			if(this.errors.length) {
				$('#preloader').fadeOut();
				//шейк окна
				$('div.fancybox-wrap').effect("shake", {times:2}, 150);
				// Включаем блок с ошибками
				$(id).find('.error').html('');
				for(var i in this.errors) {
					if(this.errors.hasOwnProperty(i)) {
						$(id).find('.error').append(this.errors[i]);
					}
				}
				$(id).find('.error').fadeIn('fast');
				this.errors = [];
				return true;
			}else{
				return false;
			}
		},
		hide : function(id){
			$(id).find('.error').html('');
			$(id).find('.error').fadeOut('fast');
		}
	}
}(jQuery);