(function($){
    $(function(){
        var staticUrl = 'static';
        // mini peckplayer definition
        swfobject.embedSWF(staticUrl + "/swf/peckplayer-mini-audio.swf",
                           "peckplayer-mini-container",
                           "160",
                           "20",
                           "10.0.0",staticUrl + "/swf/expressInstall.swf",
                           {sounds:staticUrl + "/mp3/mazik.mp3",
                            innerButtonColor:0x3c909c,
                            autostart:false,
                            autoreplay:true,
                            volume:75,
                            textColor:0x3c909c,
                            bgButtonColor:0x000000
                           },
                           {movie: staticUrl + "/swf/peckplayer-mini-audio.swf",
                            wmode: "transparent",
                            allowScriptAccess: "always"
                           },
                           {id: "peckplayer-mini-container",
                            name: "peckplayer-mini-container"
                           }
        );

        // classic peckplayer definition
        swfobject.embedSWF(staticUrl + "/swf/peckplayer-classic-audio.swf",
                           "peckplayer-classic-container",
                           "190",
                           "20",
                           "10.0.0",staticUrl + "/swf/expressInstall.swf",
                           {sounds:staticUrl + "/mp3/mazik.mp3",
                            innerButtonColor:0x3c909c,
                            autostart:false,
                            autoreplay:true,
                            volume:75,
                            textColor:0x3c909c,
                            bgButtonColor:0x000000
                           },
                           {movie: staticUrl + "/swf/peckplayer-classic-audio.swf",
                            wmode: "transparent",
                            allowScriptAccess: "always"
                           },
                           {id: "peckplayer-classic-container",
                            name: "peckplayer-classic-container"
                           }
        );

        // multi peckplayer definition
        swfobject.embedSWF(staticUrl + "/swf/peckplayer-multi-audio.swf",
                           "peckplayer-multi-container",
                           "230",
                           "20",
                           "10.0.0",staticUrl + "/swf/expressInstall.swf",
                           {sounds:staticUrl + "/mp3/mazik.mp3|/static/mp3/monautrezik.mp3",
                            innerButtonColor:0x3c909c,
                            autostart:false,
                            autoreplay:true,
                            volume:75,
                            textColor:0x3c909c,
                            bgButtonColor:0x000000
                           },
                           {movie: staticUrl + "/swf/peckplayer-multi-audio.swf",
                            wmode: "transparent",
                            allowScriptAccess: "always"
                           },
                           {id: "peckplayer-multi-container",
                            name: "peckplayer-multi-container"
                           }
        );

        $('input[class="input-color"]').change(function(event){
          $(event.target).parent().colorpicker('setValue', $(event.target).val());
        });

        $('#foreground-color').colorpicker({
            format: 'hex'
        });
        $('#background-color').colorpicker({
            format: 'hex'
        });
        $('#text-color').colorpicker({
            format: 'hex'
        });

        $('#volume').slider({
            min:0,
            max:100,
            value:80,
            slide: function( event, ui ) {
                $('#volume-value').html(ui.value);
            }
        });
        $('#volume-value').html(80);

        PeckplayerBuilder.initialize();

        var showCode = function(code){
          code = code.replace(new RegExp('<', 'g'), '&lt;');
          code = code.replace(new RegExp('>', 'g'), '&gt;');
          $('#builder-output').html(code);
          $('#builder-modal').modal();
        };

        $('#displaySwfObject').click(function(){
          var output = PeckplayerBuilder.toSWFObject();
          showCode(output);
        });

        $('#displayHTML').click(function(){
          var output = PeckplayerBuilder.toHTML();
          showCode(output);
        });

        $('#add-mp3-track').on('click', function(){
          var clone = $('.track-block:first-child').clone();
          clone.find('input').val('');
          clone.addClass('additional-track');
          $('#track-list').append(clone);
          //$('.track-block:first-child').clone().appendTo('#track-list');
        });

        var getOrigOffsetYMax = function(){
          return $('#like-block').offset().top - 40 - $('#custom-player').outerHeight();
        };

        var getOrigOffsetYMin = function(){
          return $('#builder').offset().top + 47;
        };
        var onScroll;
        enquire.register("screen and (min-width:980px)", {
          match : function (){
            onScroll();
            $(document).on('scroll', onScroll);
          },
          unmatch : function(){
            $(document).off('scroll', onScroll);
            $('#custom-player').removeClass('sticky');
            $('#custom-player').removeClass('sticky-bottom');
            $('#custom-player').css('right', '');
            $('#custom-player').css('top', '');
          },
          setup : function(){
            onScroll = function(event){
                if(window.scrollY >= getOrigOffsetYMin() && window.scrollY <= getOrigOffsetYMax() - 40){
                  $('#custom-player').css('right', $(window).width() - ($('#custom-player').offset().left + $('#custom-player').outerWidth()));
                  $('#custom-player').addClass('sticky');
                  $('#custom-player').removeClass('sticky-bottom');
                  $('#custom-player').css('top', 40);
                }
                else if(window.scrollY >= getOrigOffsetYMax() - 40){
                  $('#custom-player').css('right', $(window).width() - ($('#custom-player').offset().left + $('#custom-player').outerWidth()));
                  $('#custom-player').css('top', getOrigOffsetYMax() - window.scrollY );
                  $('#custom-player').addClass('sticky-bottom');
                  $('#custom-player').removeClass('sticky');
                }
                else{
                  $('#custom-player').removeClass('sticky');
                  $('#custom-player').removeClass('sticky-bottom');
                }
            };
          }
        });
        enquire.listen();
    });

    // fix modal bug in ie
    if ($.browser.msie) {
      $('.modal').removeClass('fade');
    }
})(jQuery);



window.NewWindow = function(page, name, w, h){
  var leftPosition = (screen.width) ? (screen.width - w) / 2 : 0;
  var topPosition = (screen.height) ? (screen.height - h) / 2 : 0;
  var settings = 'height=' + h + ',width=' + w + ',top=' + topPosition + ',left=' + leftPosition + ',scrollbars=no,resizable';
  window.open(page, name, settings);
};
