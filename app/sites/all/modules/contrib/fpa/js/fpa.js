(function ($) {
  var fpa = {
    selector : {
      form: '#user-admin-permissions',
      table : '#permissions',
      row : 'tbody tr',
      filter : 'td.permission',
      grouping : 'td.module'
    },
    dom : {
      table : '', // jquery object for entire permissions table
      rows : '', // jquery object containing just the rows
      perm_style : '', // jquery object containing permissions style
      role_style : '', // jquery object containing roles style
      section_left: '',
      section_right: ''
    },
    search:''
  };
  
  /**
   * Kick a function to the Back Of The Line.
   * 
   * Anything that is botl'd will run after other attach events, like sticky table headers.
   */
  fpa.botl = function (func) {
    setTimeout(function () {
      func();
    }, 0);
  };
  
  /**
   * Changes a string into a safe single css class.
   */
  fpa.classify = function (str) {
    return str.toLowerCase().replace(/ /ig, '-');
  };
  
  /**
   * Callback for click events on module list.
   */
  fpa.filter_module = function () {
    var $this = $(this);
    $('div.fpa-left-section ul li').removeClass('active');
    $this.parent().addClass('active');
    
    var current_perm = fpa.dom.filter.val().split('@');
    current_perm[1] = $this.attr('title');
    fpa.dom.filter.val(current_perm.join('@').replace(/@$/ig, '')); // remove trailing @ as that means no module; clean 'All' filter value
    fpa.filter('=');
  };
  
  fpa.filter = function (module_match) {
    module_match = module_match || '*=';
    perm = fpa.dom.filter.val();
    fpa.botl(function () {
      if (typeof perm != 'undefined' && ['', '@'].indexOf(perm) == -1) {
        var perm_copy = fpa.classify(perm).split('@');
        var selector = fpa.selector.table + ' ' + fpa.selector.row;
        
        var perm_style_code = selector + '[fpa-module]{display:none;}';
        
        if (perm_copy[0]) selector += '[fpa-permission*="' + perm_copy[0] + '"]';
        if (perm_copy[1]) selector += '[fpa-module' + module_match + '"' + perm_copy[1] + '"]';
        
        perm_style_code += selector + '{display: table-row;}';
        
        fpa.dom.perm_style[0].innerHTML = perm_style_code;
      }
      else {
        fpa.dom.perm_style[0].innerHTML = '';
      }
    });
  };
  
  fpa.prepare = function (context) {
    fpa.dom.form = $(fpa.selector.form, context);
    if (fpa.dom.form.length == 0) {
      return;
    }
    fpa.dom.table = fpa.dom.form.find(fpa.selector.table);
    fpa.dom.section_right = fpa.dom.table.wrap('<div class="fpa-right-section" />').parent();
    fpa.dom.module_list = $('<ul />').insertBefore(fpa.dom.section_right).wrap('<div class="fpa-left-section" />');

    // create module list
    
    fpa.dom.all_modules = $('<div />')
      .appendTo(fpa.dom.module_list)
      .wrap('<li class="active" />')
      .text('All modules')
      .attr('title', '')
      .click(fpa.filter_module);
    
    fpa.dom.table.find(fpa.selector.grouping).each(function () {
      var module_id = $(this).text();
      // Add new item to module list.
      $('<div />')
        .appendTo(fpa.dom.module_list)
        .wrap('<li />')
        .text(module_id.replace(/ module$/ig, ''))
        .attr('title', module_id)
        .attr('fpa-module', fpa.classify(module_id))
        .click(fpa.filter_module);
    });
    
    // tag rows with required classes
    fpa.botl(function () {
      fpa.dom.filter_form = $('<div class="fpa-filter-form" />').prependTo(fpa.dom.section_right);
      fpa.dom.perm_style = $('<style type="text/css" />').prependTo(fpa.dom.section_right);
      fpa.dom.role_style = $('<style type="text/css" />').prependTo(fpa.dom.section_right);
      
      // Put the "Save Permissions" button at the top and bottom.
      fpa.dom.form.find('input[type="submit"][name="op"]').remove()
        .clone().insertAfter(fpa.dom.module_list)
        .clone().insertAfter(fpa.dom.table);
        
      fpa.dom.filter = $('<input id="fpa_filter" type="text" class="form-text" placeholder="permission@module" />')
        .appendTo(fpa.dom.filter_form)
        .keypress(function (e) {
          //prevent enter key from submitting form
          if (e.which == 13) {
            return false;
          }
        })
        .keyup(function (e) {
          fpa.filter();
        })
        .wrap('<div id="fpa-filter-perm" class="form-item" />')
        .before('<label for="fpa_filter">Filter:</label>')
        .after('<div class="description">Enter in the format of permission@module;<br /> E.g. admin@system will show only permissions with<br />the text "admin" in modules with the text "system".</div>')
        .val(Drupal.settings.fpa.perm);
        
      $('<button class="clear-search">Clear Filter</button>')
        .insertAfter(fpa.dom.filter)
        .click(function (e) {
          e.preventDefault();
          fpa.dom.filter.val('');
          fpa.filter();
          fpa.dom.all_modules.click();
        });
        
      var roles_select = $('<select multiple="multiple" size=5 />')
        .appendTo(fpa.dom.filter_form)
        .wrap('<div id="fpa-filter-role" class="form-item" />')
        .before('<label for="fpa_filter">Roles:</label>')
        .after('<div class="description">Select which roles to display.<br />Ctrl+click to select multiple.</div>')
        .change(function () {
          var values = $(this).val();
          var selector_array = [];
          var role_style_code = fpa.selector.table + ' .checkbox{display:none;}';
          for (i in values) {
            selector_array.push(fpa.selector.table + ' .checkbox[title^="' + values[i] + '"]');
          }
          role_style_code += selector_array.join(',') + '{display: table-cell;}';
          
          fpa.dom.role_style[0].innerHTML = role_style_code;
        });
      
      fpa.dom.rows = fpa.dom.table.find(fpa.selector.row);
      
      var roles = fpa.dom.table.find('thead th.checkbox').each(function () {
        $this = $(this);
        var role_text = $this.text();
        var index = $this.attr('title', role_text).index() + 1;
        fpa.dom.rows.find('td:nth-child(' + index + ')').attr('title', role_text);
        $('<option />')
          .appendTo(roles_select)
          .attr('value', $this.text())
          .attr('selected', 'selected')
          .text($this.text());
      });
      
      var module_id = '';
      var $module_row;
      
      // iterate over the rows
      fpa.dom.rows.each(function () {
        var $this = $(this);
        
        // Is this a module row?
        var new_module_id = $this.find(fpa.selector.grouping).text();
        if (new_module_id != '') {
          $module_row = $this.addClass('module');
          module_id = fpa.classify(new_module_id);
        }
        var perm = $this.find(fpa.selector.filter).clone();
        perm.find('div.description').remove();
        var permission = fpa.classify($.trim(perm.text()));
        $this.attr('fpa-module', module_id).attr('fpa-permission', permission);
        $module_row.attr('fpa-permission', $module_row.attr('fpa-permission') + ' ' + permission);
      });
      
      if (Drupal.settings.fpa.perm == '' && window.location.hash.indexOf('module-') > -1) {
        fpa.dom.filter.val('@' + window.location.hash.substring(8));
        fpa.filter('=');
      }
      
      fpa.filter();
      fpa.dom.form.addClass('show');
      fpa.dom.filter.focus();
      
    });
  };
  
  fpa.modalframe = function (context) {
    $('a.fpa_modalframe', context).click(function (e) {
      e.preventDefault();
      e.stopPropagation();
      Drupal.modalFrame.open({
        url: $(this).attr('href'),
        draggable: false
      });
    });
  };
  
  Drupal.behaviors.fpa = {attach: function (context) {
    fpa.prepare(context);
    fpa.modalframe(context);
  }};
})(jQuery);
