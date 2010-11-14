// JavaScript Document

var masterArray = new Array();	//Master array to store all images
var urlArray = new Array();		//Array to store URLs 
var imgArray = new Array();		//Array to store links to images

var currentThumb = Number(0);	//Store the currentThumbnail value
var currSlideSet = 0;  			//Used to hold the slideset in view
var slidesetsArray = new Array();	//
var totalSlidesets = Number();		//Number of Slidesets
var totalThumbCount= Number(0);		//Construct name of carousel list items
//var currentThumb=0;
var loadcheck;

var scrollNum = Number();			//Number of images to scroll per move
var singleSlideWidth = Number();	//Each Image container width
var transitionSpeed = Number();		//Speed to move carousel
var viewSlide;						//Which slide to view when carosel is loaded


/*
* Add behavoir for jQuery Document Ready (Page load completion)
*
*/
Drupal.behaviors.imagefetchrbehaviors = function (context) {
	
	var carouselname = "imagefetchr_carousel";
	
			//Create instance of carousel for preview thumbnails
	thumbncarousel = new carousel(carouselname, "115");
	

	$('#'+carouselname+'_addlink_btn').bind("click", function() {
		$('#'+carouselname+'_addlink_field').toggle();
	});
		
	if (!$("#imagefetchr-checkbox-preview").attr("checked")) {
		$("#imagefetchr-display-wrapper").hide();
		$("#thumbnail_linkto-wrapper").hide();
	}
	
	if (!$("#imagefetchr-checkbox-upload").attr("checked")) {
		$("#edit-field-image-0-ahah-wrapper").hide();
	}
	
	 $("#imagefetchr-checkbox-preview").bind("click", function() {
		showHideImageFetchr(carouselname, "imagefetchr-checkbox-preview");
		//updateCarousel(carouselname);
	  });
	 
	 $("#imagefetchr-checkbox-upload").bind("click", function() {
		showHideImageFetchr(carouselname, "imagefetchr-checkbox-upload");
	  });

}; 

/*
* Create carousel object
*
*
*	@ID
*
*	@singleSlideWidth
*
*	@transitionSpeed
*	
*	@scrollnum
*	
*	@viewSlide
*
*/
function carousel(id, ssw, ts, sn, vs) {
	
	//validate arguments passed into variable
	if(!id){return false}else{var carousel = id};				//return false if no id
	if(!ssw){return false}else{singleSlideWidth = ssw};			//return false if no slide width specified
	if(!ts){transitionSpeed = 750}else{transitionSpeed = ts};	//set default if not specified
	if(!sn){scrollNum = 5}else{scrollNum = sn};					//set default if not specified
	if(!vs){viewSlide = 0}else{viewSlide = vs};	
	
		//Start carousel initiation process
	init();
		
		//Initiate carousel when created
	function init() {
		
			//Set default state of carousel
		$('#'+carousel+'_nextbutton').addClass("nextdisable");
		$('#'+carousel+'_previousbutton').addClass("previousdisable");
		$('#'+carousel+'_status').html("No previews displayed");
		$('#'+carousel+'_loading').hide();

	}
	
	
		//Click handler for refresh button
	$('#'+carousel+'_refresh_btn').click(function(){
			updateCarousel(carousel);
			return false;
	});	

	
}

	/*
	*
	*
	*/
function showHideImageFetchr(carousel, obj) {
		
	switch(obj) {
	
		case "imagefetchr-checkbox-preview":
			
				//if form checked, then display fields accordingly
			if($('#'+ obj).attr("checked")) {
				$("#imagefetchr-checkbox-upload").attr("checked", false);
				showHideImageFetchr(carousel, "imagefetchr-checkbox-upload");
				$("#imagefetchr-display-wrapper").fadeIn(500);
				$("#thumbnail_linkto-wrapper").fadeIn(500);
				updateCarousel(carousel);
			} else {
				$("#imagefetchr-display-wrapper").fadeOut(250);
				$("#thumbnail_linkto-wrapper").fadeOut(250);
			}
			
			
		break;
		
		case "imagefetchr-checkbox-upload":
			
				//if form checked, then display fields accordingly
			if($('#'+ obj).attr("checked")) {
				$("#imagefetchr-checkbox-preview").attr("checked", false);
				showHideImageFetchr(carousel, "imagefetchr-checkbox-preview");
				$("#edit-field-image-0-ahah-wrapper").fadeIn(500);
			} else {
				$("#edit-field-image-0-ahah-wrapper").fadeOut(250);
			}
			
			
		break;
	
	}
}


/*
* Cycle thumbnails forward & back in carousel
*
* @carousel - Name of carousel
*
* @e - name of arrow clicked (prev, next)
*/
function switchThumb(carousel, e) {

	switch(e) {
		case "prev":
			if(currSlideSet > 0) {	//make sure current thumb is greater than zero
				currSlideSet = currSlideSet-1;	//Decrease current thumbnail by 1
				moveCarousel(carousel, currSlideSet);
			}
		break;
		
		case "next":
			if(currSlideSet < totalSlidesets-1) {	//if current thumb less than total
				currSlideSet = currSlideSet+1;	//Increase current thumb by 1
				moveCarousel(carousel, currSlideSet);
			}
		break;
		
	}
	updateNavButtons(carousel);
	return false;
}


	/**
	*  Build video carousel
	*/
function buildVideo(arr){
	var output = "";
	$.each(arr, function(i, value){
		output += '<li><a>'+ value +'</a></li>';
	});
	$('#vidresults').html('<ul>'+output+'</ul>');
	$('#vidresults ul li a').click(function(event){
			var tmp = event.target;
			//alert($(tmp).html());
			$('#edit-field-video-0-url').val($(tmp).html());
			//selectImage(event.target);
		});
}


	/* 
	*	Handle highlighting image selected by user
	*/
function selectImage(e) {
	
	if($(e).attr("class") == "selected"){
		$(e).parent().parent().children().children().removeClass("selected");	//Remove previous selected image with class name
		$('input#imagefetchr_file').val('');
	} else {
		$(e).parent().parent().children().children().removeClass("selected");	//Remove previous selected image with class name
		$(e).addClass("selected");	//Add selected class name to image
		var url = new String($(e).attr("src"));
		$('input#imagefetchr_file').val(url);
	}
}


	/**
	* Update Carousel with images
	*/
function updateCarousel(carousel){
	$('#'+carousel+'_refresh_btn').html("Refresh Thumbnails");
	$('#'+carousel+'_statuscontainer').hide();
	$('#'+carousel+'_status').removeClass("error");
	$('#'+carousel+'_status').html("");
		
		//Construct regular expressions for url and image tags
	var regexp_url = /^((ftp|http|https):\/\/(\w+:{0,1}\w*@)?(\S+)(:[0-9]+)?(\/|\/([\w#!:.?+=&%@!\-\/]))?)/gi;	//Reg exp for urls
	var regexp_ImgTag = /(embed|img|src)=("|\')[^"\'>]+/i; //reg exp for image tags

      //Check of flushcache selected
    if ($('#flushcache').is(':checked')) {
		masterArray = new Array();  //create array
		$('#'+carousel+'_imagecontainer').html(""); //clear carousel
		totalThumbCount = 0;  //Set count to zero
		$('#'+carousel+'_imagecontainer').css("margin-left", "0px");	//move image container back to 0;
		$('#flushcache').attr('checked', false);  //reset flushcache checkbox

	}
	
		//Get image tags or image links from form
	imgArray = searchEditorField(regexp_ImgTag, "img"); 	//Get all src attributes of <img> tags on page
	
	imgArray = checkMaster(imgArray, masterArray);		//Check found urls against all matched results for duplicates
	$.merge(masterArray, imgArray);						//merge results with master array for comparision
	
		//For each image found on page, display in preview box
	$.each(imgArray, function(i, img){
      $('#'+carousel+'_imagecontainer').append(createImageTag(img, i, carousel, "std"));
      intNavBullets(carousel);
	});

    
      // Get URL/Links from editor field
	urlArray = searchEditorField(regexp_url, "a");		//Get matching urls in <a> tags on page

      //Get URL/Links from form fields
	var searchfields = Array('#'+carousel+'_addlink_field');//"input", "textarea",'.imagefetchr');

      //Go through each field with class name .imagefetchr
	$(searchfields).each(function(i,field){
      $(field).filter(function() {
        if($(this).val().match(regexp_url)){
          urlArray.push($(this).val().match(regexp_url));
        }
      });
	});

	urlArray = checkMaster(urlArray, masterArray);	//Check found urls against all matched results for duplicates
	$.merge(masterArray, urlArray);  				//merge results with master array for comparision

      //Loads images from array into preview container
	if(urlArray.length > 0) {
      $.each(urlArray, function(i, url){
        loadImages(url, carousel);
      });
	} 
		
		/* 
		*	Check urlArray for values, if found display loading message with details
		*/
	var loadcount = 0;
	var loadmsg="";
	loadcheck = setInterval(function() {
    if(urlArray.length > 0) {
      if(loadcount >=60) {
        loadmsg = "Giving up on "+ urlArray;
        /*setTimeout(function() {
            $('#'+carousel+'_statuscontainer').fadeOut();
            $('#'+carousel+'_status').removeClass("error");
            $('#'+carousel+'_status').html("");
        }, 3000);*/
        clearInterval(loadcheck);
      } else if(loadcount >=5) {
        loadmsg = "Still loading "+ urlArray;
      } else {
        loadmsg = "Loading "+ urlArray;
      }
      $('#'+carousel+'_status').html(loadmsg);
      $('#'+carousel+'_loading').show();
      $('#'+carousel+'_statuscontainer').show();
      loadcount++
    } else if (urlArray.length <= 0) {
      $('#'+carousel+'_status').html("");
      $('#'+carousel+'_statuscontainer').hide();
      clearInterval(loadcheck);
    }
  }, 1000);

  if(urlArray.length <= 0 && imgArray.length <= 0) {
    $('#'+carousel+'_statuscontainer').show();
    $('#'+carousel+'_status').addClass("error");
    $('#'+carousel+'_status').html("No new previews found.");
    /*setTimeout(function() {
        $('#'+carousel+'_statuscontainer').fadeOut();
        $('#'+carousel+'_status').removeClass("error");
        $('#'+carousel+'_status').html("");
    }, 3000);*/
  }

}


	



	/**
	* Calculate Navigation Bullets
	*
	* Summary: detects how many slide items, then determines how many navigation bullets to create based on the ScrollNumber variable
	*/
function calcNavBullets(){
	var totalBullets;
	
	if (totalThumbCount % scrollNum == 0){
			//If total node count divides evenly with number of slides to scroll
		totalBullets = totalThumbCount / scrollNum;
		return totalBullets;
	} else {
			//If cannot divide evenly, then add value to allow proper rounding
		totalBullets = Math.round(totalThumbCount / scrollNum + .5);
		return totalBullets;
	}
}

	/*
	*	moveCarousel
	*	Moves the carousel items back and forth
	*
	*	@carousel = object name of carousel to control
	*
	*	@currSlideSet = Current slide set to move to
	*/
function moveCarousel(carousel, currSlideSet) {
		$('#'+carousel+'_imagecontainer').animate(
				{marginLeft:''+slidesetsArray[currSlideSet]+'px'},
				{duration: ''+transitionSpeed+'', easing: 'easeInOutQuad'}
		);
		updateNavButtons(carousel);
}


	/**
	* Update carousel navigation buttons & indicator
	*/
function updateNavButtons(carousel) {
	
	if(totalSlidesets <= 1) {
		$('#'+carousel+'_bulletcontainer').hide();
	} else {
		$('#'+carousel+'_bulletcontainer').show();
	}
	
	if(currSlideSet == 0) {
		$('#'+carousel+'_previousbutton').addClass("previousdisable");
		//$('#'+carousel+'_previousbutton').removeClass("previousbutton");
	} else {
		$('#'+carousel+'_previousbutton').removeClass("previousdisable");
		//$('#'+carousel+'_previousbutton').addClass("previousbutton");
	}
	
	if(currSlideSet >= totalSlidesets-1) { 
		$('#'+carousel+'_nextbutton').addClass("nextdisable");
		//$('#'+carousel+'_nextbutton').removeClass("nextbutton");
	} else {
		$('#'+carousel+'_nextbutton').removeClass("nextdisable");
		//$('#'+carousel+'_nextbutton').addClass("nextbutton");
	}
	
	
	//Loop through for each slideset group to determine which should be disabled/enabled
	for(i=0;i<slidesetsArray.length;i++) {
			//check if current slideset group matches loop variable
		if(currSlideSet == i) {
			$('#'+carousel+'_bullet'+i).addClass("current");  //if there is match, set CSS classname to disable
		} else {
			$('#'+carousel+'_bullet'+i).removeClass("current");	//for all other nav bullets, set CSS classname to enable them
		}
	}
	
	$('#'+carousel+'_bulletcontainer').width(totalSlidesets * 25+"px");
	
}


	/*
	*	Prepares and formats image for output to page
	*/
function createImageTag(img, image_id, carouselId, json){
	var maxWidth = Number(100);
	var maxHeight = Number(85);
	var newImg;
	var height;
	var width;
	totalThumbCount++;	//Increase totalThumbnail count
	
	switch(json){
		case "json":
			height = img.height;
			width = img.width;
			img = img.url;
		break;
		
		case "std":
			newImg = new Image();
			newImg.src = img;
			height = newImg.height;
			width = newImg.width;
		break;
		
		default:
			newImg = new Image();
			newImg.src = img;
			height = newImg.height;
			width = newImg.width;
		break;
		
	}
	var rwidth;
	var rheight;
		
	if((height>maxHeight) || (width > maxWidth)){
		var ratio = Math.min(Number(maxWidth)/Number(width), Number(maxHeight)/Number(height));
		rwidth = ratio * width;
		rheight = ratio * height;
	}
	
	output = '<div class="thumbnail" id="'+carouselId+'_thumb'+ image_id +'"><img src="' +img+ '" style="width:'+rwidth+';height:'+rheight+'px;"><div class="thumbnail-meta">'+width +'x'+ height+'px</div></div>';
	return output;
}


	/**
	* Load all images sent to function
	*/
function loadImages(url, carousel) {
	
	var maxWidth = Number(100);		//Set width of thumbnail in carousel
	var maxHeight = Number(85);		//Set height of thumbnail in carousel
	var screenshot = false;	
	
	//$('#'+carousel+'_imagecontainer').html("");
	if ($('#screenshot').is(':checked')) {
		screenshot = true;
	}

	$.post("/imagefetchr", {"url[]": url, "screenshot":screenshot}, function(json){
		
			//Check if there are array types named thumbnail
		if(json.thumbnails){
			var output = "";
			
				//Cycle through each thumbnail and format for page
			$.each(json.thumbnails,function(i,thumb) {
				output += createImageTag(thumb, i, carousel, "json")
			});
			
				//Append new image to end of carousel 
			$('#'+carousel+'_imagecontainer').append(output);
			
				//call initial bullets to reset
			intNavBullets(carousel);
			
		}
		
			//Display error if found
		if(json.error) {
			$('#'+carousel+'_loading').hide();	//Hide loading icon
			$('#'+carousel+'_status').html(json.error);
			$('#'+carousel+'_status').addClass("error");
		}
		
			//Used for loading detector: Go through original array and remove urls that match
		$.each(urlArray, function(x, value) {
			if(escape(value) == escape(url)) {
				urlArray.splice(x, 1);
				//alert("removed: "+ value);
			}						
				
		});
	}, 'json');

}





	


	/*
	*	Initiate navigation bullets
	*/
function intNavBullets(carousel) {
	
	$('#'+carousel+'_previousbutton').unbind('click');
	$('#'+carousel+'_nextbutton').unbind('click');
	$('#'+carousel+' .thumbnail img').unbind('click');
	$('#'+carousel+'_bulletcontainer a').unbind('click');
	$('#'+carousel+'_bulletcontainer').html("");
		
		//Determine number of slideset by dividing total slides by scrollNum
	//currSlideSet = 0;	//Reset current Slideset
	slidesetsArray = Array();
	totalSlidesets = calcNavBullets();  //totalSlidesets = Math.round(slideArray.length / scrollNum);
	
		// loop through total number of slidesets, assign position into array and create scroll nav bullets
	for(i=0; i < totalSlidesets; i++){
		slidesetwidth = i * scrollNum
		slidesetsArray[i] = -slidesetwidth * singleSlideWidth;
		
		if(currSlideSet == i) {
			$('#'+carousel+'_bulletcontainer').append('<a id="'+carousel+'_bullet'+i+'" class="navbullets current">&nbsp;</a>');	//insert new bullet into HTML page
		}
		else {
			$('#'+carousel+'_bulletcontainer').append('<a id="'+carousel+'_bullet'+i+'" class="navbullets">&nbsp;</a>');	//insert new bullet into HTML page
		}
	} 
	
		//Assign Click handler for thumbnails
	$('#'+carousel+' .thumbnail img').click(function(event){
		selectImage(event.target);
	});
	
	$('#'+carousel+'_nextbutton').removeClass("nextdisable");
	$('#'+carousel+'_previousbutton').removeClass("previousdisable");
			
		//assign click handler for previous button
	$('#'+carousel+'_previousbutton').click(function(){
		switchThumb(carousel, "prev"); 
	});
	
		//Assign click handler for next button
	$('#'+carousel+'_nextbutton').click(function(){
		switchThumb(carousel, "next");
	 });
	 
		//Assign click handler for bullets
	$('#'+carousel+'_bulletcontainer a').click(function(event){
		var str = new String(event.target.id);
		currSlideSet =  Number(str.trimTo(1, true));
		moveCarousel(carousel, currSlideSet);
	});
	
	$('#'+carousel+'_navindicate').html("("+ totalThumbCount +" found)");
	updateNavButtons(carousel);
}

	/**
	* Check provided array again master and return only unmatched results
	*/
function checkMaster(resultArray, masterArray) {
	
	if(resultArray){
		//Check resultArray against cache
		$.each(masterArray, function(x, val){
		
			$.each(resultArray, function(i, value) {
					//alert("val1: "+ val +", val2: "+ value);
					if(escape(val) == escape(value)) {
						resultArray.splice(i, 1);
						//alert("removed: "+ value);
					}					 
			});					
		});
		return resultArray;
	}

}


	/**
	*  Search FCKEditor field for any URLS
	*/
function searchEditorField(regexp, lookfor) {
	
    var resultArray = Array();	//Set container for matched results
    
    if(typeof(FCKeditorAPI) != 'undefined') {
        //Get FCKeditor content
      var oEditor = FCKeditorAPI.GetInstance('edit-body');	//Get FCK object on page
      var strFCKEditorHTML = oEditor.GetHTML();	//Get HTML from FCK editor
      var oDOM = oEditor.EditorDocument;
      var strFCKEditorText = "";

      if (document.all){
          // If I.E.
          strFCKEditorText = oDOM.body.innerText;
      } else {
          //All other browsers
          var r = oDOM.createRange();
          r.selectNodeContents(oDOM.body);
          strFCKEditorText = r.toString();
      }

          //Check FCK HTML for <a href tags>
      if(strFCKEditorHTML) {

          var linkArray;
          switch(lookfor){
              case "a":
                  linkArray = parseHTML(strFCKEditorHTML, "a");	//return array of all <a href> tags
              break;

              case "img":
                  linkArray = parseHTML(strFCKEditorHTML, "img");
              break;

              case "embed":
                  linkArray = parseHTML(strFCKEditorHTML, "embed");
              break;
          }
          //var imgArray = parseHTML(strFCKEditorHTML, "img");	//return array of all <a href> tags
          //$.merge(linkArray, imgArray);

          //var tmpArray = strFCKEditorText.split(" ");
          //alert(tmpArray);

          $.each(linkArray, function(i,word) {
              resultArray.push(word);
          });

      }

          //Check FCK text for URLs
      if(strFCKEditorText) {

          var splitregex = /\s|<\/p>/;
          var txtArray = strFCKEditorText.split(splitregex);
          //alert(txtArray.length);
          $.each(txtArray, function(i,word) {
                  if(word.match(regexp)) {
                      resultArray.push(word);
                  }
          });
      }

      return resultArray;
    }

}

	/*
	*	Search HTML for all <A> tags and return their href value
	*/
function parseHTML(html, tag) {
	var imgArray = Array();
	$(html).find(tag).each(function() {
		switch(tag){
		 	case "a":
				imgArray.push($(this).attr("href"));
			break;
			
			case "img":
				imgArray.push($(this).attr("src"));
			break;
			
			case "embed":
				imgArray.push($(this).attr("src"));
			break;
			
			default:
				imgArray.push($(this));
			break;
		}
	});

	return imgArray;
}


	//Adds TrimTo to string objects
	// @len :: Number of characters to reduce the string to.
	// @atEnd :: Blank, or false to work at beginning of string, true to work at end of string.
String.prototype.trimTo = function(len, atEnd){
   var rex = new RegExp( (atEnd ? ('^.*(.{' + len + '})$') : ('^(.{' + len + '}).*$')), 'gim');
   return this.replace(rex, '$1');
}