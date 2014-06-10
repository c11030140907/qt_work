/****************************************************************************
**
****************************************************************************/

import QtQuick 2.2
import QtQuick.Window 2.1
import QtQuick.Controls 1.2
import QtQuick.XmlListModel 2.0

Window {
	visible: true
	width: 538 + frame.margins * 2
	height: 360 + frame.margins * 2
 
	SystemPalette {id: syspal}
	color: syspal.window

	function setData(lst){
		lst.forEach(function(elem){
			largeModel.append({"path ":elem })
		});
	}

	ListModel {
		id: largeModel
		Component.onCompleted: {}
	}

	Component {
		id: delegate1
		Item {
			clip: true
			Text {
				width: parent.width
				anchors.margins: 4
				anchors.left: parent.left
				anchors.verticalCenter: parent.verticalCenter
				elide: styleData.elideMode
				text: styleData.value !== undefined  ? styleData.value : ""
				color: styleData.textColor
			}
		}
	}

	TableView {
		id: frame
		model: largeModel
		property int margins: 1
		objectName: "tbl"
		anchors.margins: 12
		anchors.fill:parent
		headerVisible: true
		alternatingRowColors: true

		function test01(){
			console.log("hello!");
			console.log(appData.getCurrentDateTime());
		}
		
		function test02(arr, obj) {
			arr.forEach(function(elem){console.log("Array_item:",elem);});
			for(var key in obj){console.log("Object item:",key,obj[key]);}
		}

		TableViewColumn {
			role: "path"
			title: "Path"
			width: 120
		}

		headerDelegate: BorderImage{
			source: "images/header.png"
			border{left:2;right:2;top:2;bottom:2}
			Text {
				text: styleData.value
				anchors.centerIn:parent
				color:"#333"
			}
		}

		rowDelegate: Rectangle {
			height: 30
			Behavior on height{ NumberAnimation{} }

			color: styleData.selected ? "#448" : (styleData.alternate? "#eee" : "#fff")
			BorderImage{
				id: selected
				anchors.fill: parent
				source: "images/selectedrow.png"
				visible: styleData.selected
				border{left:2; right:2; top:2; bottom:2}
			}
		}
		itemDelegate: {
			return delegate1;
		}
	}
}
