/**
*	Класс для управления генерацией PDF файлов
*
*	@author Mark V. Lvov
*	@email lvov.web@gmail.com
*/

var pdfgenerate = (function () {

	return {
		
		//Флаг остановки генерации
		stopped: true,
		
		init: function (doc_id, key, hdaccess) {
			
			var _this = this;
			
			_this.doc_id = doc_id,
			_this.key = key,
			_this.hdaccess = typeof hdaccess !== "undefined" ? hdaccess : false,
			_this.stopped = false,
			
			//config
			_this.debug = false;//co
			_this.interval = 2;//sec. ping inteval
			_this.timeOut = 900;//sec timeout to generate PDF
			_this.emailTimeout = 5;//sec. to show email messeges/status

			_this.pdfButtonHeandler();
			_this.emailButtonHeadler();
			_this.setPDFButtonState("default");

		},
		
		//остановка конвертации
		stop: function () {
			
			var _this = this;
			
			_this.stopped = true;
			
		},
		
		//Слушает событие клик на кнопке PDF
		pdfButtonHeandler: function () {

			var _this = this;

			$(".buttonPDF_download").unbind().click(function(){

				_this.generatePDF();

			});

		},       

		//Слушает событие клик на кнопке отправки email (нужно в случае ошибки/разрыва связи/таймаута)
		emailButtonHeadler: function () {

			var _this = this;

			$(".buttonPDF_send_input.textarea").bind("click",function(){

				$(".buttonPDF_send_input.textarea").css("display","none");
				$(".buttonPDF_send_input.input").css("display","block");
				$(".buttonPDF_send_input.input").focus();

			});

			$(".buttonPDF_send_input.input").bind("blur",function(){

				var value = $(".buttonPDF_send_input.input").val();

				if(value == ""){
					$(".buttonPDF_send_input.input").css("display","none");
					$(".buttonPDF_send_input.textarea").css("display","block");
				}

			});

			$(".buttonPDF_submit").unbind().bind("click",function(){
				
				if(!$(this).hasClass("disabled")){
					_this.sendEmail();
				}

				return false;
			});

		},

		//Запускает генерацияю PDF
		generatePDF: function () {

			var _this = this;

			$.ajax({
				url: '/pdf/get/', 
				data: {id: _this.doc_id, key: _this.key,hdaccess:_this.hdaccess},
				dataType: 'json',
				type: 'POST',
				beforeSend: function(){

					if(_this.debug){console.log("Start check PDF...")};

					_this.setPDFButtonState("check");

				},
				success: function (data,textStatus){

					if(_this.debug){console.log("Response textStatus: "+textStatus)};

					switch(data['status']){

						case 'ready':

							if(_this.debug){console.log('PDF is generated before...')};

						case 'inprocess':

							if(_this.debug){console.log('PDF is generate now by other user...')};

						case 'started':

							if(_this.debug){console.log('Starting ping server...')};

							_this.setPDFButtonState("start");

							var timeStart = Math.floor(new Date().getTime() / 1000);

							_this.checkReady(timeStart);

						break;

						case 'err':

							switch(data['msg']){

								case 'badlink':

									if(_this.debug){console.log('Bad PDF key...')};

									_this.setPDFButtonState("badkey");

								break;
								
								case 'unauthorized':
								
									if(typeof APP !== "undefined"){
										
										_this.setPDFButtonState("default");
										APP.putMSG('act',{key:"session_timeout"});
										
									}

									
								break;
								
								default:

									if(_this.debug){console.log('Unknowm error (api)... ')};

									_this.setPDFButtonState("email");

								break;
							}

						break;

					}

				},
				error: function (XMLHttpRequest, textStatus, errorThrown) {

					if(_this.debug){
						console.log(XMLHttpRequest);
						console.log(textStatus);
						console.log(errorThrown);
					}

					_this.setPDFButtonState("email");

				}
			});

		},

		//Рекурсивно пингует сервер и проверяет готовность PDF, до готовности или таймаута
		checkReady: function (timeStart) {

			var _this = this;

			var timeNow = Math.floor(new Date().getTime() / 1000);
			var showedPercent = $(".buttonPDF_progress .bar span").text().replace(/ %/g, '');

			$.get("/pdf/get/",{id:_this.doc_id,key:_this.key,hdaccess:_this.hdaccess},function(data){

				switch(data['status']){

					case 'inprocess':

						if(_this.debug){console.log('Percent complite: '+data['percent']+'%')};

						if(data['percent'] == 100 && showedPercent == 100){

							if(_this.debug){console.log('Ending...')};

							_this.setPDFButtonState("complite");

						}else{
							//Запускаем псевдоанимацию процентов
							_this.showPercentProgress(data['percent']);

						}

						//Проверяем превышение таймаута
						if((timeNow - _this.timeOut - _this.interval) > timeStart){

							if(_this.debug){console.log('PDF time OUT! Stoped... Show email form')};

							_this.setPDFButtonState("email");

						}else{
							
							if(!_this.stopped){
								setTimeout(function(){//continue ping

									_this.checkReady(timeStart);

								}, _this.interval * 1000);
							}

						}
						
					break;

					case 'ready':
						
						if(!_this.stopped){
							
							if(_this.debug){console.log('PDF generate is COMPLITE!')};

							if(showedPercent != 100){

								if(_this.debug){console.log("Show animation 0% to 100% in interval time...")};

								_this.showPercentProgress(100);

								setTimeout(function(){

									if(_this.debug){console.log('Getting a file')};

									_this.setPDFButtonState("complite");

									setTimeout(function(){

										if(_this.debug){console.log('Return to default state')};

										_this.setPDFButtonState("default");

									},_this.interval*1000);//Time to get file

									//window.location = "/pdf/getfile/id/"+_this.doc_id+"/key/"+_this.key;
									window.location = "/pdf/get/id/"+_this.doc_id+"/key/"+_this.key+"/file/1";

									if(($.browser.msie) && (($.browser.version == '8.0')||($.browser.version == '7.0'))){
									$('.buttonPDF').remove();
									$('#content-wrapper').append("<div id='ie8-download-link'>Если загрузка не началась автоматически <a id='dwlink' href='/pdf/getfile/id/"+_this.doc_id+"/key/"+_this.key+"'>нажмите ссылку</a></div>");
										var oper = document.getElementById("switch-content4-low");
										if(oper != null){
											$("#ie8-download-link").css('width','300px');
										}
									}

								},_this.interval*1000);//Time to show pseudo progress

							}else{

								if(_this.debug){console.log("Show complite");};

								_this.setPDFButtonState("complite");

								setTimeout(function(){

									if(_this.debug){console.log('Return to default state...')};

									_this.setPDFButtonState("default");

								},_this.interval*1000);//Time to get file

								if(_this.debug){console.log('Getting a file')};

								window.location = "/pdf/get/id/"+_this.doc_id+"/key/"+_this.key+"/file/1";

							}
						}

					break;

					case 'err':

						switch(data['msg']){

							case 'badlink':

								if(_this.debug){console.log('Bad PDF key...')};

								_this.setPDFButtonState("badkey");

							break;

							case 'checkerror':

								if(_this.debug){console.log('Ping error, show email form...')};

								_this.setPDFButtonState("email");

							break;

							default:

								if(_this.debug){console.log('Unknowm error...')};

								_this.setPDFButtonState("email");

							break;
						}
					break;
				}
			},'json');

		},


		//Устанавливает отображение состояния кнопки
		setPDFButtonState: function (state) {

			var _this = this;

			if(_this.debug){console.log('setPDFButtonState: '+state)};

			$(".buttonPDF_progress .bar").css("width","0%");
			$(".buttonPDF_progress .bar span").text("0 %");
			$(".buttonPDF_download").css("marginTop","0");

			var marginTop=$(".buttonPDF_download").height();

			switch(state){//set need

				case 'default':

					$(".buttonPDF_download").css("marginTop","0");

				break;

				case 'check':

					$(".buttonPDF_download").css("marginTop","-"+marginTop*2+"px");

				break;

				case 'start':

					$(".buttonPDF_download").css("marginTop","-"+marginTop*1+"px");

				break;

				case 'badkey':  

					$(".buttonPDF_download").css("marginTop","-"+marginTop*4+"px");

				break;

				case 'email':

					$(".buttonPDF_download").css("marginTop","-"+marginTop*5+"px");

				break;

				case 'email_bad':

					$(".buttonPDF_download").css("marginTop","-"+marginTop*7+"px");

				break;

				case 'email_ok':

					$(".buttonPDF_download").css("marginTop","-"+marginTop*6+"px");

				break;

				case 'complite':

					$(".buttonPDF_download").css("marginTop","-"+marginTop*3+"px");

				break;

			}

			return true;

		},

		//Показыает анимацию процентов выполнения генерации
		showPercentProgress: function (newPercent) {

			var _this = this;

			var curPercent = $(".buttonPDF_progress .bar span").text().replace(/ %/g, '');

			var difference = newPercent - curPercent;

			var timeToPercent = (_this.interval/difference) * 1000;



			var intervalPercent = setInterval(function() {

				var updPercent = parseInt($(".buttonPDF_progress .bar span").text().replace(/ %/g, ''));

				var showPercent = updPercent + 1;

				if(showPercent <= newPercent){

					$(".buttonPDF_progress .bar").css("width",showPercent+"%");
					$(".buttonPDF_progress .bar span").text(showPercent+" %");

				}else{

					clearInterval(intervalPercent);

					return true;
				}
			}, timeToPercent);


		},

		//Отправляет email 
		sendEmail: function () {

			var _this = this;

			var email = $(".buttonPDF_send_input.input").val();

			var reg = /[a-z0-9!#$%&'*+/=?^_`{|}~-]+(?:.[a-z0-9!#$%&'*+/=?^_`{|}~-]+)*@(?:[a-z0-9](?:[a-z0-9-]*[a-z0-9])?.)+[a-z0-9](?:[a-z0-9-]*[a-z0-9])?/;

			if(email != "" && email.match(reg)){

				$.ajax({
					url: '/pdf/mail/', 
					data: {id: _this.doc_id, key: _this.key, email: email},
					dataType: 'json',
					type: 'POST',
					beforeSend: function(){

						$(".buttonPDF_submit").addClass("disabled");

					},
					success: function (data,textStatus){

						if(_this.debug){console.log("Response textStatus: "+textStatus)};

						$(".buttonPDF_submit").removeClass("disabled");

						switch(data['status']){

							case 'ok':

								_this.setPDFButtonState("email_ok");

								setTimeout(function(){

									_this.setPDFButtonState("default");

								}, _this.emailTimeout * 1000);//Time to show ok msg

							break;

							case 'err':

								switch(data['msg']){

									case 'empty':

									case 'invalid':

										_this.setPDFButtonState("email_bad");

										setTimeout(function(){

											_this.setPDFButtonState("email");

										}, _this.emailTimeout * 1000);//Time to show err msg

									break;

									default:

										_this.setPDFButtonState("badkey");

										setTimeout(function(){

											_this.setPDFButtonState("email");

										}, _this.emailTimeout * 1000);//Time to show err msg

									break;

								}

							break;

						}

					},
					error: function (XMLHttpRequest, textStatus, errorThrown) {

						$(".buttonPDF_submit").removeClass("disabled");

						if(_this.debug){
							console.log(XMLHttpRequest);
							console.log(textStatus);
							console.log(errorThrown);
						}

						_this.setPDFButtonState("badkey");

					}
				});

			}else{

				_this.setPDFButtonState("email_bad");

				setTimeout(function(){

					_this.setPDFButtonState("email");

				}, _this.emailTimeout * 1000);//Time to show err msg

			}

		}


	}

})();