$(document).ready(function() {
    order_doc.init();

    hash = location.hash;
    if(hash == '#zakaz_demonstracii') // Подключение заказа демонстрации
        $('.order_doc').click();

});
var order_doc = (function () {
    return {
        init: function () {
            //Переменная указывает был ли уже сабмит формы
            this.submit_login = false;
            //Подключение фэнсибокса
            this.fancyboxHandler();
            //скрытие ошибок при фокусе
            this.focusInput();
            //хендлер обработки формы регистрации
            this.initderdocFormHandler();
            $(".order_doc").on('touchend', function() {
                $(this).trigger('click');
                return false;
            });
        },
        fancyboxHandler: function () {
            var _this = this;
            var $fi = ''
            var ph = ''
            // Заказ демонстрации
            $(".order_doc").fancybox({
                maxWidth    : 540,
				padding		: 0,
                fitToView    : false,
                width        : '70%',
                height        : '75%',
                autoSize    : true,
                closeClick    : false,
                openEffect    : 'fade',
                'scrolling' : 'no',
                href        : '/user/orderdocform',
                type        : 'ajax',
                beforeShow : function() {
                    $fi = $('.fancybox-inner')
                    ph = $fi.height()
                    this.title = null;

                    location.hash = '#zakaz_demonstracii';
                },
                afterShow : function() {
                    $fi.height(ph)
                    var $regionOrderdoc = $('#regionOrderdoc')
                    $regionOrderdoc.focus()
                    $(document).on("submit", "#form-orderdoc", function(){
                        $fi.css('overflow','visible')
                    });
                    function regionChanged(){
                        var $address = $('#address'),
							city = '';
                        $('#wrappOrderdoc').css('margin-top','');
                        switch($('#regionOrderdoc').val()){
                            case '1':
                                city = 'Москва'
                                break;
                            case '2':
                                city = 'Санкт-Петербург'
                                break;
                            case 'other':
                                $('#wrappOrderdoc').css('margin-top','50px')
                                break;
                        }
                        $address.val(city).html(city);
                    }
                    function activityChanged(){
                        var class1 = 'forSeniorShow'
                        var class2 = 'forSenior'
                        if ($('#activityOrderdoc').val()>2){
                            class1 = 'forSenior'
                            class2 = 'forSeniorShow'
                        }
                        $('.'+class1).each(function(i,el){
                            $(el).removeClass(class1).addClass(class2)
                        })
                    }
                    $('#regionOrderdoc').on('change',regionChanged)
                    $('#activityOrderdoc').on('change',activityChanged)
                },
                afterClose: function () {
                    location.hash = '';
                }
            });
        },
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
            $('.formOrderdoc').on('focus', 'input[type="text"]', function(){
                Errors.hide('#'+$(this).closest('.fancybox-inner').children('div').attr('id'));
            });
        },
        /*    Регистрация пользователя    */
        initderdocFormHandler: function () {
            var _this = this;
            //Подрубаем куфон
            Cufon.replace(".submitOrderdoc");
            //Фокус на форме
            $('.formOrderdoc').on('focus', 'input[type="text"]', function(){
                Errors.hide('#'+$(this).closest('.fancybox-inner').children('div').attr('id'));
            });
            //инициализация обработки формы
            _this.registrateHandler();
            //Инициализация кнопок формы после окончания регистрации
            _this.compliteRegButtonsHandler();
        },
        //Отправка формы
        registrateHandler: function () {
            var _this = this;
            $(document).on('submit', '#form-orderdoc', function(){
                var data = _this.checkOrderdocForm();
                if(data){
                    _this.registrateUser(data);
                }
                return false;
            });
        },
        //Валидация формы на фронте
        checkOrderdocForm: function () {
            var _this = this;
            Errors.hide('#dialogOrderdoc');
            var region = $('#dialogOrderdoc .region option:selected').val()
            var other_region = false;
            if(region == 'other'){
                var region = $('#dialogOrderdoc #drugoi').val()
                other_region = true;
            }
            var data = {
                other_region : other_region,
                region: region,
                city: $('#address').val(),
                activity: $('#dialogOrderdoc #activityOrderdoc option:selected').val(),
                fullname: $('#dialogOrderdoc input[name=fio]').val(),
                email: $('#dialogOrderdoc input[name=email]').val(),
                phone_country: '+7',
                phone_code: $('#dialogOrderdoc input[name=phone_code]').val().replace(/\s+/g,''),
                phone_number: $('#dialogOrderdoc input[name=phone_number]').val().replace(/[\s-]/g,''),
                company : $('#dialogOrderdoc input[name=company]').val(),
                deyatelnost : $('#dialogOrderdoc input[name=deyatelnost]').val(),
                position : $('#dialogOrderdoc input[name=position]').val(),
                comment : $('#dialogOrderdoc input[name=comment]').val(),
                docId : $('.order_doc').attr('doc_id')
            };
            //Тесты
            // Проверяем e-mail
            var reg = /^([A-Za-z0-9_\-\.])+\@([A-Za-z0-9_\-\.])+\.([A-Za-z]{2,4})$/;
            if(reg.test(data.email) == false) {
                Errors.add('email','Некорректный&nbsp;e-mail');
            }
            if(region.length == ''){
                Errors.add('region','Выберите&nbsp;регион');
            }
            if(data.activity.length == ''){
                Errors.add('activity','Выберите&nbsp;деятельность');
            }
            if(data.city.replace(/(^\s+)|(\s+$)/g, '') == ''){
                Errors.add('city','Пожалуйста,&nbsp;укажите&nbsp;город');
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

	        // Проверяем организацию
	        if($.inArray( parseInt(data.activity), [ 3,4,5 ] ) != -1 && data.company.length < 1) {
		        Errors.add('company','Пожалуйста,&nbsp;укажите&nbsp;организацию');
	        }

            //Если были ошибки, выходим
            if(Errors.view('#dialogOrderdoc')){
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
                url: '/user/orderdoc',
                cache: false,
                dataType: 'json',
                beforeSend: function(){
                    $('#orderdocloader').css('display','')
                },
                success: function(response) {
                    $('#orderdocloader').fadeOut();
                    if(response && response.result == true) {
                        $.fancybox({
                            maxWidth    : 540,
                            padding        : 0,
                            fitToView    : false,
                            width        : '70%',
                            height        : '75%',
                            autoSize    : true,
                            closeClick    : false,
                            openEffect    : 'fade',
                            'scrolling' : 'no',
                            href        : '/user/orderdocformres/?result=true',
                            type        : 'ajax',
                            closeEffect    : 'fade',
                            beforeShow : function() {
                                $('html').css('overflow-y','hidden')
                            },
                            beforeClose : function(){
                                $('html').css('overflow-y','')
                            }
                        })
                    }else{
                        var description = response.message;
                        var err_messages = response.message.split('invalid ')[1]
                        if (err_messages == undefined){
                            $.fancybox({
                                maxWidth    : 540,
                                padding        : 0,
                                fitToView    : false,
                                width        : '70%',
                                height        : '75%',
                                autoSize    : true,
                                closeClick    : false,
                                openEffect    : 'fade',
                                'scrolling' : 'no',
                                href        : '/user/orderdocformres/?result=false',
                                type        : 'ajax',
                                closeEffect    : 'fade',
                                beforeShow : function() {
                                    $('html').css('overflow-y','hidden')
                                },
                                beforeClose : function(){
                                    $('html').css('overflow-y','')
                                }
                            })
                        }else{
                            err_messages = err_messages.split(', ');
                            var l = err_messages.length
                            for (var i=0; i<l; i++){
                                var err_message = err_messages[i]
                                switch (err_message){
                                    case 'email':
                                        description = 'Некорректный&nbsp;e-mail'
                                        break;
                                    case 'region':
                                    case 'other_region':
                                        description = 'Выберите&nbsp;регион'
                                        break;
                                    case 'activity':
                                        description = 'Выберите&nbsp;деятельность'
                                        break;
                                    case 'city':
                                        description = 'Пожалуйста,&nbsp;укажите&nbsp;город'
                                        break;
                                    case 'name':
                                        description = 'Пожалуйста,&nbsp;укажите&nbsp;ФИО'
                                        break;
                                    case 'phone':
                                        description = 'Некорректный&nbsp;номер&nbsp;телефона'
                                        break;
	                                case 'company':
		                                description = 'Пожалуйста,&nbsp;укажите&nbsp;организацию'
		                                break;
                                    default:
                                        err_message = 'unknown'
                                        description = response.message
                                }
                                Errors.add(err_message,description)
                            }
                            Errors.view('#dialogOrderdoc')
                        }
                    }
                },
                error: function () {
                    $('#loader').fadeOut();
                    $.fancybox({
                        maxWidth    : 540,
                        padding        : 0,
                        fitToView    : false,
                        width        : '70%',
                        height        : '75%',
                        autoSize    : true,
                        closeClick    : false,
                        openEffect    : 'fade',
                        'scrolling' : 'no',
                        href        : '/user/orderdocformres/?result=error',
                        type        : 'ajax',
                        closeEffect    : 'fade',
                        beforeShow : function() {
                            $('html').css('overflow-y','hidden')
                        },
                        beforeClose : function(){
                            $('html').css('overflow-y','')
                        }
                    })
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
                maxWidth    : 540,
                maxHeight    : 600,
                padding        :0,
                fitToView    : false,
                width        : '70%',
                height        : '70%',
                'scrolling' : 'no',
                autoSize    : true,
                closeClick    : false,
                openEffect    : 'fade',
                helpers : {
                    title:  null
                },
                closeEffect    : 'fade'
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
