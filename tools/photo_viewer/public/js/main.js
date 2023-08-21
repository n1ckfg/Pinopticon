"use strict";

var camNameBegin = "ws://";
var camNameList = [ "nfg-rpi-4-2", "nfg-rpi-3-4", "nfg-rpi-3-3", "nfg-rpi-2-2", "nfg-rpi-2-1", "nfg-rpi-0-1" ];
var camNameEnd = ".local:7112";
var camUrls = [];
var camWs = [];
var stillBoxes = [];
var fileList = [];
var activeCameras = 0;

// ~ ~ ~ ~ ~ ~ ~ ~ 

function resetList() {
	fileList = [];
}

function takePhoto() {
	resetList();
	console.log("Taking photos...");
	for (var i=0; i<camWs.length; i++) {
		camWs[i].send("take_photo");
	}
}

function makeCamUrls() {
	for (var i=0; i<camNameList.length; i++) {
		var camUrl = camNameBegin + camNameList[i] + camNameEnd;

		camUrls.push(camUrl);
	}
}

function openCamConnections() {
	for (var i=0; i<camUrls.length; i++) {
		console.log("Attempting to open " + camUrls[i]);

		var ws = new WebSocket(camUrls[i]);
		camWs.push(ws);
	}	

	for (var i=0; i<camWs.length; i++) {
		camWs[i].onopen = function(evt) { onOpen(evt) };
		camWs[i].onclose = function(evt) { onClose(evt) };
		camWs[i].onmessage = function(evt) { onMessage(evt) };
	}
}

function slowMode() {
    for (var i=0; i<camWs.length; i++) {
        camWs[i].send("update_slow");
    }  
    console.log("Live feed in slow mode.");
}

function fastMode() {
    for (var i=0; i<camWs.length; i++) {
        camWs[i].send("update_fast");
    }  
    console.log("Live feed in fast mode.");
}

// ~ ~ ~ ~ ~ ~ ~ ~ 

function onOpen(evt) {
	activeCameras++;
	console.log("Active Cameras: " + activeCameras);
	if (activeCameras > camUrls.length) console.log("Error: counted too many active cameras.")
}

function onClose(evt) {
	//activeCameras--;
	//console.log("Active Cameras: " + activeCameras);
	//if (activeCameras < 0) console.log("Error: counted too few active cameras.")
}

function onMessage(evt) {
	var results = evt.data.split(",");
	console.log(results);
	for (var i=0; i<camNameList.length; i++) {
		if (results[0] == camNameList[i]) {
			var url = "http://" + camNameList[i] + ".local:7110/photos/" + results[1];
			console.log("RESPONSE: " + url);
			
			//var filename = url.split("/")[url.split("/").length-1];
			//download(filename, url);
			
			fileList.push(url);
			if (fileList.length >= activeCameras) {
				sendFileList(fileList);
				resetList();
			}

			/*
			var thumbnail = document.getElementById(camNameList[i] + "_img");
			// https://stackoverflow.com/questions/14595541/capture-div-into-image-using-html2canvas
			html2canvas(thumbnail, {
		        useCORS: true,
		        allowTaint: true,
		        onrendered: function(canvas) {
					stillBoxes[i].style.backgroundImage = "url(\"" + "data:image/png;base64," + encodeBase64Image(thumbnail, "png") + "\")";
					stillBoxes[i].style.backgroundSize = "100px 75px";	
		        }				
			});
			*/
			
			break;
		}
	}
}

function main() {
	makeCamUrls();
	openCamConnections();
	setupKeys();

	for (var i=0; i<camNameList.length; i++) {
		var sb = document.getElementById("sb"+(i+1));
		stillBoxes.push(sb);
	}
}

window.onload = main;