'use strict';

/*
Put any interaction code here
 */

// Selectors
var $ = ["inputBtn", "analysisBtn", "inputDiv", "analysisDiv", "logo", "inputSubmit", "inputForm", "activitySelect", "viewForm", "selectGraph", "metrics", "errorPrompt", "genData"];

window.addEventListener('load', function() {
    var storeModel = new ActivityStoreModel();
    var graphModel = new GraphModel();

    storeModel.addListener(lastEntryView);
    storeModel.addListener(tableView);
    storeModel.addListener(graphView);
    storeModel.addListener(buttonsView);

    graphModel.addListener(selectGraphView);

    graphModel.selectGraph(0);

    _.each($, function(e) {
        $[e] = document.getElementById(e);
    });

    _.each(["input", "analysis"], function(e, i) {
        var opposite = (i == 0 ? "analysis" : "input");

        $[e + "Btn"].addEventListener('click', function(bt) {
            this.parentNode.className = "active";
            $[opposite + "Btn"].parentNode.className = "";

            $[e + "Div"].className = "";
            $[opposite + "Div"].className = "hidden";

            bt.preventDefault();
        });
    });

    $["activitySelect"].addEventListener("change", function() {
       updateGraph();
    });

    // Click on logo does what is expected
    $["logo"].addEventListener("click", function() {
        $["inputBtn"].click();
        $["inputForm"].reset();
    });

    // Generate fake data
    $["genData"].addEventListener("click", function(e) {
        e.preventDefault();
        generateFakeData(storeModel, 5);
    });

    // Input validation
    $["inputSubmit"].addEventListener("click", function(e) {
        e.preventDefault();

        var data = { };
        var error = null;

        // Validation

        data.activity = $["inputForm"].activity.value;
        data.time = parseInt($["inputForm"].time.value);
        data.healthMetrics = {
            energyLevel: parseInt($["inputForm"].energy.value) || 1,
            happinessLevel: parseInt($["inputForm"].happiness.value) || 1,
            stressLevel: parseInt($["inputForm"].stress.value) || 1
        };

        /// Activity and time cannot be empty
        if(!data.activity) {
            error = "Please specify the activity you wish to record";
        }

        /// Time is an integer, minimum 0
        else if(isNaN(data.time) || data.time < 0) {
            error = "The time spent must be a positive integer";
        }

        /// Health metrics are integers [1-10] (silent error, since users shouldn't really mess with these)
        _.map(data.healthMetrics, function(i) {
            return (i > 0 && i <= 10) ? i : 1;
        });

        // Data
        if(!error) {
            // Hide the error prompt if any
            $["errorPrompt"].className = "hidden";

            // Add activity data point
            storeModel.addActivityDataPoint(new ActivityData(data.activity, data.healthMetrics, data.time));

            // Go to the analysis tab
            $["inputForm"].reset();
        }
        else {
            $["errorPrompt"].className = "";
            $["errorPrompt"].getElementsByTagName("span")[0].innerHTML = error;
        }

        console.log(storeModel);
    });

    // Select graph
    var radios = $["selectGraph"].getElementsByTagName("input");
    _.each(radios, function(r) {
        r.addEventListener("click", function() {
           graphModel.selectGraph(r.value);
        });
    });
});