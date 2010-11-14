
/**
 * Attach handlers to count the number of words in a given textfield, in its
 * description.
 */
$(document).ready(function() {
  
/*  $("#edit-socialmediafeeds-status").keyup(function() {
    var charsLeft = (140 - $(this).val().length);
    var descDiv = $(this).parent().children(".description");
    var descDivHTML = $(descDiv).html();
	$(descDiv).html("<<strong>" + charsLeft + "</strong> characters remaining");
    if (charsLeft < 0) {
      $(descDiv).addClass("negative");
      $("#twitter-post-button").attr('disabled', 'true');
    } else {
      $(descDiv).removeClass("negative");
      $("#twitter-post-button").removeAttr('disabled');
    }
  });*/
  
/*	$.each('#socialmediafeeds-fieldset .form-checkbox', function(row, value){
		if(!$(row).attr("checked")) {
			
		}													 
	});*/
  if (!$("#Twitter-toggle").attr("checked")) {
    $("#Twitter-account-wrapper").hide();
  }
  
  if (!$("#Facebook-toggle").attr("checked")) {
    $("#Facebook-account-wrapper").hide();
  }
  
  if (!$("#Linkedin-toggle").attr("checked")) {
    $("#Linkedin-account-wrapper").hide();
  }

  $("#Twitter-toggle").bind("click", function() {
    if ($("#Twitter-toggle").attr("checked")) {
      $("#Twitter-account-wrapper").show();
    }
    else {
      $("#Twitter-account-wrapper").hide();
    }
  });
  
   $("#Facebook-toggle").bind("click", function() {
    if ($("#Facebook-toggle").attr("checked")) {
      $("#Facebook-account-wrapper").show();
    }
    else {
      $("#Facebook-account-wrapper").hide();
    }
  });
   
  $("#Linkedin-toggle").bind("click", function() {
    if ($("#Linkedin-toggle").attr("checked")) {
      $("#Linkedin-account-wrapper").show();
    }
    else {
      $("#Linkedin-account-wrapper").hide();
    }
  });
  
});
