var dialog		= window.parent ;
var oEditor		= dialog.InnerDialogLoaded() ;
var FCK			= oEditor.FCK ;
var FCKLang		= oEditor.FCKLang ;
var FCKConfig	= oEditor.FCKConfig ;

//security RegExp
var REG_SCRIPT = new RegExp("< *script.*>|< *style.*>|< *link.*>|< *body .*>", "i");
var REG_PROTOCOL = new RegExp("javascript:|vbscript:|about:", "i");
var REG_CALL_SCRIPT = new RegExp("&\{.*\};", "i");
var REG_EVENT = new RegExp("onError|onUnload|onBlur|onFocus|onClick|onMouseOver|onMouseOut|onSubmit|onReset|onChange|onSelect|onAbort", "i");
// Cookie Basic
var REG_AUTH = new RegExp("document\.cookie|Microsoft\.XMLHTTP", "i");
// TEXTAREA
var REG_NEWLINE = new RegExp("\x0d|\x0a", "i");

//#### Dialog Tabs

// Set the dialog tabs.
//dialog.AddTab( 'Info', oEditor.FCKLang.DlgInfoTab ) ;

// Get the selected flash embed (if available).
var oFakeImage = FCK.Selection.GetSelectedElement() ;
var oEmbed ;

$(document).ready(function() {

	$.post('/socialmediafeedaccounts', {type:'statusfeed'}, function(data){
		$('#resultcontainer').html(data);
	},'html');
	
});

window.onload = function()
{
	// Translate the dialog box texts.
	//oEditor.FCKLanguageManager.TranslatePage(document) ;

	dialog.SetAutoSize( true ) ;
	dialog.SetOkButton( true ) ;	// Activate the "OK" button.

}

//#### The OK button was hit.
function Ok()
{
	
    oEditor.FCKUndo.SaveUndoStep() ;
    var output = "";
    
	//Get all checkboxes with class .socialmedia
	$('.socialmedia:checked').each(function(i, value){
			//var value = $(this).val().split("=");
			//alert($('#count'+i).val());
			output += createStatusFeedOutput($(this).val(), $('#count'+i).val(), $('#theme'+i).val());
			
			//output += createHTML(value[0], value[1])
			//alert(value[1]);								
	});
	
	FCK.InsertHtml(output);
	
	//FCK.InsertHtml(createHTML(GetE('twitterurl').value, GetE('twitterfeed').value));

	return true ;
}

function createStatusFeedOutput(service, count, theme){
	var output = '[statusfeed '+ service +' count='+ count +' theme='+ theme +']';
	return output;
}


function checkCode(code)
{
	if (code.search(REG_SCRIPT) != -1) {
		return false;
	}
	
	if (code.search(REG_PROTOCOL) != -1) {
		return false;
	}

	if (code.search(REG_CALL_SCRIPT) != -1) {
		return false;
	}

	if (code.search(REG_EVENT) != -1) {
		return false;
	}

	if (code.search(REG_AUTH) != -1) {
		return false;
	}

	if (code.search(REG_NEWLINE) != -1) {
		return false;
	}
}