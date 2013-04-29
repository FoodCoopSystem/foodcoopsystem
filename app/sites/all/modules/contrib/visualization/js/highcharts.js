(function($) {

Drupal.behaviors.visualization_highcharts = {
  attach: function(context) {
    $.each($(".visualization-chart-highcharts", context).not(".visualization-processed"), function(idx, value) {
      var chart_id = $(value).attr("id");
      var chart = Drupal.settings.visualization[chart_id];

      $(value).addClass("visualization-processed");

      if (chart !== undefined) {
        Drupal.visualization.charts[chart_id] = new Highcharts.Chart(chart.options);

        Drupal.visualization.charts[chart_id].resize = function(width, height) {
          this.setSize(width, height, false);
        }
      }
    })
  },
  detach: function(context) {
  }
};

})(jQuery);
