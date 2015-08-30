(function($){

    var PeckplayerBuilder = (function(){
        var innitialized, container, sizes,
            type, textColor, innerColor, backgroundColor, autoStart, loop, volume, sounds,
            draw, onChangeType, onChangeInnerColor, onChangeAutoPlay, onChangeLoop, onChangeVolume, staticUrl, peckplayerUrl;

        staticUrl = 'static';
        peckplayerUrl = 'static/swf';
        sizes = {mini:'160', classic:'190', multi:'230'};
        container = 'custom-player-flash';
        innitialized = false;
        type = 'mini';
        textColor = '3c909c';
        innerColor = '3c909c';
        backgroundColor = '000000';
        autoStart = false;
        loop = true;
        volume = 80;
        //sounds = 'http://peckplayer.scopart.fr/static/mp3/mazik.mp3';
        sounds = '';


        draw = function(){
            // clear container
            $('#custom-player-flash').empty();
            swfobject.embedSWF(staticUrl + '/swf/peckplayer-' + type + '-audio.swf',
                               container,
                               sizes[type],
                               '20',
                               '10.0.0',staticUrl + '/swf/expressInstall.swf',
                               {sounds:sounds,
                                innerButtonColor:'0x' + innerColor,
                                autostart:autoStart,
                                autoreplay:loop,
                                volume:volume,
                                textColor:'0x' + textColor,
                                bgButtonColor:'0x' + backgroundColor
                               },
                               {movie: staticUrl + '/swf/peckplayer-' + type + '-audio.swf',
                                wmode: 'transparent',
                                allowScriptAccess: 'always'
                               },
                               {id: container,
                                name: container
                               }
            );
        };

        onChangeType = function(){
            type = $(this).val();
            draw();
            if($(this).val() == 'multi'){
                $('#add-mp3-track').css('display','inline-block');
            }else{
                $('.additional-track').remove();
                $('#add-mp3-track').css('display','none');
            }
        };

        onChangeInnerColor = function(ev){
            innerColor = ev.color.toHex().replace('#', '');
            $("#custom-player-flash").get(0).setInnerColor(innerColor);
        };

        onChangeBackgroundColor = function(ev){
            backgroundColor = ev.color.toHex().replace('#', '');
            $("#custom-player-flash").get(0).setBackgroundButtonColor(backgroundColor);
        };

        onChangeTextColor = function(ev){
            textColor = ev.color.toHex().replace('#', '');
            $("#custom-player-flash").get(0).setTextColor(textColor);
        };

        onChangeAutoPlay = function(){
            autoStart = $(this).is(':checked');
            draw();
        };

        onChangeLoop = function(){
            loop = $(this).is(':checked');
            draw();
        };

        onMP3ListChange = function(){
            sounds_array = [];
            $('input[name="mp3-track"]').each(function(){
                sounds_array.push($(this).val());
            });
            sounds = makeSoundList(sounds_array);
            draw();
        };

        onChangeVolume = function(event, ui){
            volume = ui.value;
            draw();
        };

        function makeSoundList(soundtracks){
            var soundlist = '';
            for (var i = 0;
                i < soundtracks.length; i++) {
                if(soundtracks[i] !== ''){
                    if(soundlist !== ''){
                        soundlist += '|http://' + soundtracks[i];
                    }else{
                        soundlist += 'http://' + soundtracks[i];
                    }
                }
            }
            return soundlist;
        }

        return {
            initialize : function(){
                // check if initialized
                if(!innitialized){
                    innitialized = true;

                    // initialize event listeners
                    $('input[name="peckplayer-type"]').change(onChangeType);
                    $('#foreground-color').colorpicker().on('changeColor', onChangeInnerColor);
                    $('#background-color').colorpicker().on('changeColor', onChangeBackgroundColor);
                    $('#text-color').colorpicker().on('changeColor', onChangeTextColor);
                    $('input[name="autoplay"]').change(onChangeAutoPlay);
                    $('input[name="loop"]').change(onChangeLoop);
                    //$('input[class="mp3-track"]').change(onMP3ListChange);
                    $('#track-list').change(onMP3ListChange);
                    $('#volume').slider().on('slide', onChangeVolume);

                    // draw with default values
                    draw();
                }
            },

            toHTML : function(){
                var output = '<object style="visibility: visible;" width="' + sizes[type] + '" height="20" id="peckplayer" name="peckplayer">\n';
                output += '    <param name="movie" value="' + peckplayerUrl + '/peckplayer-' + type + '-audio.swf" />\n';
                output += '    <param name="wmode" value="transparent" />\n';
                output += '    <param name="allowscriptaccess" value="always" />\n';
                output += '    <param name="flashvars" value="sounds=' + sounds + '&amp;innerButtonColor=0x'+ innerColor +'&amp;autostart=' + autoStart + '&amp;autoreplay=' + loop + '&amp;volume=' + volume + '&amp;textColor=0x' + textColor + '&amp;bgButtonColor=0x' + backgroundColor + '" />\n';
                output += '    <!--[if !IE]>-->\n';
                output += '    <object type="application/x-shockwave-flash" data="' + peckplayerUrl + '/peckplayer-' + type + '-audio.swf" width="' + sizes[type] + '" height="20">\n';
                output += '        <param name="wmode" value="transparent" />\n';
                output += '        <param name="allowscriptaccess" value="always" />\n';
                output += '        <param name="flashvars" value="sounds=' + sounds + '&amp;innerButtonColor=0x' + innerColor + '&amp;autostart=' + autoStart + '&amp;autoreplay=' + loop + '&amp;volume=' + volume + '&amp;textColor=0x' + textColor + '&amp;bgButtonColor=0x' + backgroundColor + '" />\n';
                output += '    <!--<![endif]-->\n';
                output += '        <a href="http://www.adobe.com/go/getflashplayer"><img src="http://www.adobe.com/images/shared/download_buttons/get_flash_player.gif" alt="Get Adobe Flash player" /></a>\n';
                output += '    <!--[if !IE]>-->\n';
                output += '    </object>\n';
                output += '    <!--<![endif]-->\n';
                output += '</object>';
                return output;
            },

            toSWFObject : function(){
                var output = '<script type="text/javascript">\n';
                output += 'var flashvars = {\n';
                output += '     sounds:"' + sounds + '",\n';
                output += '     innerButtonColor:0x' + innerColor + ',\n';
                output += '     autostart:' + autoStart + ',\n';
                output += '     autoreplay:' + loop + ',\n';
                output += '     volume:' + volume + ',\n';
                output += '     textColor:0x' + textColor + ',\n';
                output += '     bgButtonColor:0x' + backgroundColor + '\n';
                output += '};\n';
                output += 'var params = {\n';
                output += '     movie: "' + peckplayerUrl + '/peckplayer-' + type + '-audio.swf",\n';
                output += '     wmode: "transparent",\n';
                output += '     allowScriptAccess: "always"\n';
                output += '};\n';
                output += 'var attributes = {\n';
                output += '     id: "flashContainer",\n';
                output += '     name: "flashContainer"\n';
                output += '};\n';
                output += 'swfobject.embedSWF("' + peckplayerUrl + '/peckplayer-' + type + '-audio.swf", "flashContainer", "' + sizes[type] + '", "20", "10.0.0","expressInstall.swf", flashvars, params, attributes);\n';
                output += '</script>\n';
                output += '<div id="flashContainer">\n';
                output += '     <p>Contenu alternatif</p>\n';
                output += '</div>\n';
                return output;
            }
        };
    })();

    window.PeckplayerBuilder = PeckplayerBuilder;

})(jQuery);