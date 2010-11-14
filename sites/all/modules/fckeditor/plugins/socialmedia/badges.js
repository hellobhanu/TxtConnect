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


//dialog.AddTab( 'Twitter', oEditor.FCKLang.DlgInfoTab ) ;	 // Set the dialog tabs.

//dialog.AddTab( 'Facebook', oEditor.FCKLang.DlgInfoTab ) ;	 // Set the dialog tabs.


var oActiveEl = dialog.Selection.GetSelectedElement() ;


$(document).ready(function() {

	$.post('/socialmediafeedaccounts', {type:'badge'}, function(data){
		$('#resultcontainer').html(data);
	},'html');
	
});

window.onload = function()
{
	dialog.SetAutoSize( true ) ;
	dialog.SetOkButton( true ) ;
	
	
}

//#### The OK button was hit.
function Ok()
{
	var error = false;
	var errormsg = "";
	var output = "";
	
	oEditor.FCKUndo.SaveUndoStep() ;

		//Get all checkboxes with class .socialmedia
	$('.socialmedia:checked').each(function(){
			var value = $(this).val().split("|");
			output += createBadgeTag(value[0], value[1])
			//alert(value[1]);								
	});
	
	FCK.InsertHtml(output);
	return true;

	//oEditor.FCKEmbedAndObjectProcessor.RefreshView(oFakeImage, oEmbed);

	
}

function createBadgeTag(sName, sUrl) {
	var imgUrl;
	switch(sName) {
	
		case "Twitter":
			imgUrl = "/sites/default/files/badge_twitter.jpg";
		break;
		
		case "Facebook":
			imgUrl = "/sites/default/files/badge_facebook.jpg";
		break;
		
		case "Linkedin":
			imgUrl = "/sites/default/files/badge_linkedin.gif";
		break;
	}
	
	return '<a href="'+sUrl+'" target="_blank"><img src="'+ imgUrl +'" alt="Follow me on '+ sName +'" border="0"/></a>';
}


function Update(e, sid)
{
	
	switch(sid) {
		
		case "Twitter":
			e.src = "/sites/default/files/badge_twitter.jpg";
		break;
		
		case "Facebook":
			e.src = "/sites/default/files/badge_facebook.jpg";
		break;
		
		case "Linkedin":
			e.src = "/sites/default/files/badge_linkedin.gif";
		break;
	}
	SetAttribute(e, 'border', "0" ) ;
	
		//Create link tag to surround image tag
	oEditor.FCKSelection.SelectNode( e ) ;
	oLink = oEditor.FCK.CreateLink( GetE(sid).value )[0] ;
	SetAttribute( oLink, 'target', "_blank" ) ;
	SetAttribute( oLink, 'alt', "Follow me on "+ sid ) ;

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