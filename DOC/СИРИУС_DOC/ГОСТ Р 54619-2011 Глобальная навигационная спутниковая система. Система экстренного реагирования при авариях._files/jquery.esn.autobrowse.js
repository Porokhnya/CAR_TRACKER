/**
 * Version 2.0
 *
 * Written by Micael Sjölund, ESN (http://www.esn.me)
 *
 * Creates a growing container that automatically fills its content via ajax requests, when the user scrolls to the
 * bottom of the container. More info: http://pushingtheweb.com/2010/09/endless-scroller-jquery-plugin/
 *
 * Requires jStorage (), if the useCache option is set to true. WARNING: Somewhat experimental. See below for more info.
 *
 *
 * Usage:
 * .autobrowse(options)
 *    options   Map of property-value options which controls plugin behaviour.
 * .autobrowse(command)
 *    command   String command that can be sent to the plugin.
 *
 *
 * * COMMANDS
 * * "flush"        Clears the plugin cache
 *
 *
 * * OPTIONS
 * * url            Callback to render url from offset argument.
 *                  Example: function (offset) { return "http://mydomain.com/OFFSET/20".replace(/OFFSET/, offset) }
 * * template       Callback to render markup from json response.
 *                  Example: function (response) { var markup=''; for (var i=0; i<response.length; i++) { markup+='<img src="'+response[i]+'" />' } return markup; }
 * * itemsReturned  Callback that is run on ajax json response to determine how many items was returned
 *
 * * OPTIONAL OPTIONS
 * * loader         Element, jQuery object or markup to indicate to the user that the site is waiting for more items.
 * * offset         Item offset for first ajax call to url, if you have already rendered items on the page
 * * max            Maximum number of items to be retrieved by the plugin (can also be used to tell the plugin how many
 *                  items there are in total on the server, so that no unneccesary requests are made.
 * * complete       Callback that is run when the element has been updated with new content. This is run before the
 *                  response is stored (if using useCache), making it is possible to manipulate the response here before
 *                  it is stored.
 * * sensitivity    Number of pixels before end of element that the plugin will start fetching more items.
 * * postData       If you want to do a POST request instead of a GET request, supply this argument, either as a
 *                  function or an object. If not set, a GET request will be made.
 * * useCache       If true, the plugin will use browser storage to keep the state between page loads. If the user
 *                  clicks away from the page and then goes back, all items fetched will be rendered again, and the
 *                  user will see the same view as when he left the page. Requires http://www.jstorage.info/.
 *                  WARNING: This doesn't work with original jStorage. A modified version is
 *                  available on http://github.com/msjolund/jquery-esn-autobrowse. jStorage also
 *                  requires jquery-json: http://code.google.com/p/jquery-json/. Default: false
 * * expiration     How long to keep cache, in hours. Default: 24
 *
 */
(function( $ ){
jQuery.fn.autobrowse = function (options)
{
    var defaults = {
        url: function (offset) { return "/"; },
        before: function (response) { return ""; },
        template: function (response) { return ""; },
        after: function (response) { return ""; },
        offset: 0,
		offsetUp: 1,
        max: null,
		json: false,
        loader: '<div class="loader"></div>',
        itemsReturned: function (response) { return response.length },
        complete: function (response) {},
        finished: function (response) {},
        useCache: false,
        expiration: 24,
        sensitivity: 0,
		up : 0,
        postData: null,
		stopFunction: function () {},
		error_template: '',
		onError: function () {
			if(options.loader){
				options.loader.hide();
				options.loader.before(options.error_template);
				options.loader.remove();
			}
		}
    };

    // flush cache command
    if (typeof options == "string" && options == "flush")
    {
		currentOffset = 0;
		jQuery.jStorage.flush();
		
		if(typeof(HandlerScrollCallback) != 'undefined'){
			jQuery(window).unbind("scroll",HandlerScrollCallback);
			HandlerScrollCallback = '';
		}
		if(typeof(HandlerScrollCallbackUp) != 'undefined'){
			jQuery(window).unbind("scroll",HandlerScrollCallbackUp);
			HandlerScrollCallbackUp = '';
		}
        return this;
    }

    options = jQuery.extend(defaults, options);

    // allow non-dynamic url
    if (typeof options.url == "string")
    {
        var url = options.url;
        options.url = function (offset) { return url; }
    }

    var getDataLength = function (data)
    {
        var length = 0;
        for (var i = 0; i < data.length; i++)
            length += options.itemsReturned(data[i]);
        return length;
    };

    return this.each( function ()
    {
        var localData, obj = jQuery(this);
		if(typeof(currentOffset) == 'undefined'){
		  currentOffset = options.offset;
		}
        var loading = false;
		var loader = false;
        var scrollTopUpdateTimer = null;
		var stopping = false;
		
        var _stopPlugin = function (handler)
        {
            jQuery(window).unbind("scroll", handler);
            options.finished.call(obj);
        };

		var scrollCallback = function ()
		{
			// костыль чтобы не начинал грузить контент если див-вкладка скрыт
			if ($(obj).is(':visible'))  {
				
				var scrollTop = jQuery(window).scrollTop();
				if(options.up){
					//Прокрутка сверух от самого элемента на котором повешен скролер
					var objUp = obj.offset().top;
	
					if (scrollTop  < objUp + options.sensitivity && !loading){
						_scroll('up');
					}
				}else{
					var objBottom = obj.height() + obj.offset().top;
					var winHeight = window.innerHeight ? window.innerHeight : $(window).height();
					var winBtmPos = scrollTop + winHeight;
					if (scrollTopUpdateTimer)
						clearTimeout(scrollTopUpdateTimer);
					if (options.useCache) {
						scrollTopUpdateTimer = setTimeout(function () { jQuery.jStorage.set("autobrowseScrollTop", scrollTop); }, 200);
					}
					if (objBottom  < winBtmPos + options.sensitivity && !loading){
						_scroll('down');
					}
				}
	
			}
				
		};

		var _scroll = function(direction){

			var loader = jQuery(options.loader);
			if(direction == 'up'){
				loader.prependTo(obj);
				var offset =  options.offsetUp;
			}else{
				loader.appendTo(obj);
				var offset =  options.offset;
			}
			loading = true;
			
			var ajaxCallback = function (response) {
				if (options.itemsReturned(response) > 0){
					if(options.json){
						var all = response.all
						response = response.html;
					}else{
						var all = 0;
					}
				
					// Create the markup and append it to the container
					try { 
						var markup = options.template(response, options.offset); }
					catch (e) { console.error(e) } // ignore for now
					var newElements = $(markup);
					
					options.before(newElements); //Действия до
					
					if(options.json && all){
						obj.html(newElements);
						_stopPlugin(scrollCallback)
						stopping = true;
					}else{
						if(direction == 'up'){
							var heightBefore = 0;
							if(options.gluetable && $(loader).next().is('table:not([id])') && $(newElements).last().is('table')){
								heightBefore = $(document).outerHeight(true);
								$(loader).next().find('tr:first').before(newElements.last().find('tbody:last').html());
								newElements = newElements.not('table:last');
							}else {
								heightBefore = $(document).outerHeight(true);
								
							}
							$(newElements).prependTo(obj);

							var heightAfter = $(document).outerHeight(true);

							var difference = heightAfter - heightBefore;
							window.scrollBy(0, difference - loader.outerHeight(true));

							options.merge = false;
							options.height_before_merge = 0;
						}else{
							if(options.gluetable && $(loader).prev().is('table:has([id])') && $(newElements).first().is('table:not([id])')){
								$(loader).prev().find('tr:last').after(newElements.first().find('tbody:last').html());
								newElements = newElements.not('table:first');
							}

							$(newElements).appendTo(obj);
							
						}
					}

					options.after(newElements); //Действия после

					// Call user onComplete callback
					options.complete.call(obj, response, newElements);

					// Store in local cache if option is set, and everything fetched fitted into the storage
					if (options.useCache && direction != 'up' && getDataLength(localData) + options.offset == currentOffset)
					{
						localData.push(response);
						if (!jQuery.jStorage.set("autobrowseStorage", localData))
							// Storage failed, remove last pushed response
							localData.pop();
					}

					// Update offsets
					if(direction != 'up'){
						currentOffset += options.itemsReturned(response);
					}else{
						options.offsetUp -= options.itemsReturned(response);
					}
					if (options.useCache && direction != 'up')
					{
						jQuery.jStorage.set("autobrowseOffset", currentOffset);
					}
				}
				loader.remove();
				
				// Check if these were the last items to fetch from the server, if so, stop listening
				if (options.itemsReturned(response) == 0 || (options.max != null && currentOffset >= options.max) || options.stopFunction(response) === true )
				{
					_stopPlugin(scrollCallback)
					stopping = true;
				}
				loading = false;
				if (!stopping) {
					scrollCallback();
				}
			};
			
			if (options.postData){
				var data = null;
				if (typeof options.postData == "function"){
					data = options.postData();
				}
				else{
					data = options.postData;
				}

				jQuery.post(options.url(currentOffset), data, ajaxCallback, "json").error(options.onError);
			}else{
				if(options.json){
					if(options.up){
						jQuery.getJSON(options.url(options.offsetUp), ajaxCallback).error(options.onError);
					}else{
						jQuery.getJSON(options.url(currentOffset), ajaxCallback).error(options.onError);
					}
				}else{
					if(options.up){
						jQuery.ajax({url : options.url(options.offsetUp), dataType: "html", success:ajaxCallback,error:options.onError});
					}else{
						jQuery.ajax({url : options.url(currentOffset), dataType: "html", success:ajaxCallback,error:options.onError});
					}
				}
			}
			
			
			
		}

        var _startPlugin = function()
        {                
            if (options.useCache)
                var autobrowseScrollTop = jQuery.jStorage.get("autobrowseScrollTop");
            if (autobrowseScrollTop){
                jQuery(window).scrollTop(autobrowseScrollTop);
			}
				jQuery(window).scroll(scrollCallback);
			if(options.up){
				HandlerScrollCallbackUp = scrollCallback;
			}else{
				HandlerScrollCallback = scrollCallback;
			}
            scrollCallback();
        };


        if (options.useCache)
        {
            if (jQuery.jStorage.get("autobrowseStorageKey") != options.url(0,0))
            {
                // flush cache if wrong page
				 jQuery.jStorage.flush();
            }
            else if (jQuery.jStorage.get("autobrowseExpiration") && jQuery.jStorage.get("autobrowseExpiration") < (new Date()).getTime())
            {
                // flush cache if it's expired
				 jQuery.jStorage.flush();
            }
            localData= jQuery.jStorage.get("autobrowseStorage");
            if (localData)
            {
                // for each stored ajax response
                for (var i = 0; i < localData.length; i++)
                {
                    var markup = options.template(localData[i]);
                    jQuery(markup).appendTo(obj);
                    currentOffset += options.itemsReturned(localData[i]);
                    options.complete.call(obj, localData[i]);
                }
                _startPlugin();
            }
            else
            {
                localData = [];
                jQuery.jStorage.get("autobrowseStorageKey")
                jQuery.jStorage.set("autobrowseExpiration", (new Date()).getTime()+options.expiration*60*60*1000);
                jQuery.jStorage.set("autobrowseOffset", currentOffset);
                jQuery.jStorage.set("autobrowseStorageKey", options.url(0, 0));
                jQuery.jStorage.set("autobrowseStorage", localData);
                jQuery.jStorage.set("autobrowseScrollTop", 0);
                _startPlugin();
            }
        }

        else
        {
            _startPlugin();
        }
    });
};
})( jQuery );