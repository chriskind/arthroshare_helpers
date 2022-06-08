<?php

class arthroshareDatesStepsAlerts {
  

  public $deadline_interval;
  
  
  /*
   * alert functions
   */

  function arthroshare_get_alert_levels($case_study_id = NULL) {
    
    $alert_numbers = array(
        5, 
        2, 
        0
    );
    $alert_text = array(
        'Reminder', 
        'Urgent Reminder', 
        'Researcher Call'
    );
    
    $alert_levels = array();
    
    foreach($alert_numbers as $key => $number) {
      $text = $alert_text[$key];
      $class = str_replace(" ", "-", strtolower($text));
      
      $alert_levels[$number] = array(
          'text' => $text, 
          'class' => $class
      );
    }
    return $alert_levels;
  }
  
  function arthroshare_get_alert_level($alert_level, $alert_levels) {
    $n = $alert_level;
    // print_r($n);
    while(!in_array($n, array_keys($alert_levels)) && $n >= 1 ) {
      $n = $n - 1;
    }
    return $n;
  }

  
  /*
   * dates & date interval functions
   */

  function arthroshare_get_due_date($case_start_date, $deadline_interval, $case_start = NULL) {
    
    if(!isset($case_start) | empty($case_start)) {
      $case_start = new DateTime("$case_start_date");
      $next_step = new DateTime("$case_start_date");
    } else {
      $next_step = $case_start;
    }

    
    $now = new DateTime();
    
    $next_step_due = $this->arthroshare_format_interval($deadline_interval, 'object');
    $next_step->add($next_step_due);
    $checkdue = $now->diff($next_step);
    
    $in_days = ($checkdue->format('%r%a'));
    
    $due_date_info = array('in_days' => $in_days, 'due_date' => $next_step->format('m-d-Y'), 'next_step' => $next_step);
    
    return $due_date_info; 
  }
  
  function arthroshare_format_interval($interval_data = array('interval' => 1, 'period' => 'day'), $return_type = NULL, $return_format = '%Y %M %D') {
    
    $input_interval = $interval_data['interval'];
    $input_period = $interval_data['period'];
    
    $dti_period = array(
        'day' => array(
            'prefix' => 'P', 
            'postfix' => 'D'
        ), 
        'week' => array(
            'prefix' => 'P', 
            'postfix' => 'W'
        ), 
        'hour' => array(
            'prefix' => 'PT', 
            'postfix' => 'H'
        ), 
        'seconds' => array(
            'prefix' => 'PT', 
            'postfix' => 's'
        )
    );
    
    $interval_string = $dti_period[$input_period]['prefix'] . $input_interval . $dti_period[$input_period]['postfix'];
    
    if($return_type == 'dti_string') {
      return $interval_string;
    }  
    
    $output_object = new DateInterval($interval_string);
    
    if($return_type == 'object') {
      return $output_object;
    } else { 
      return $output_object->format($return_format);
    }
  }
  
  function arthroshare_case_get_due_dates($case_study_nids = array(), $return_type = 'next_due') {
    // retrieve all cases & cycle a foreach
  

  // retrieve all steps and cycle
  

  }

  /*
   * step functions
   */
  
  function arthroshare_get_protocol_steps($protocol_nid) {
    
    $protocol_info = node_load($protocol_nid);
    $protocol_steps = $protocol_info->field_protocol_step[LANGUAGE_NONE];
    return $protocol_steps;
  }
  
  function athroshare_get_protocol_steps_info($case_study_nid, $return = array()) {
    $case_node = node_load($case_study_nid);
    $protocol_node = adhd_field_output('field_protocol', 'node', $case_node);
    $protocol_nid = $protocol_node->nid;
    
    if(!isset($return) | empty($return)) {
      return($protocol_nid);
    }
    
  }

  function arthroshare_get_protocol_step_deadline($nid, $format = NULL, $return_type = NULL) {
    
    if((!isset($nid) | empty($nid))) {
      return ("no node id");
    }
    
    $node = node_load($nid);

    if((!isset($node->type) | empty($node->type))) {
      return("no node type");
    }
    
    $protocol_step_deadline = adhd_field_output('field_protocol_step_deadline', 'node', $node);
    
    if(!isset($format) && !isset($return_type)) {
      return $this->arthroshare_format_interval($protocol_step_deadline);
    } elseif(isset($return_type) && $return_type == 'raw') {
        return $protocol_step_deadline;
    } elseif(isset($return_type) && $return_type == 'object') {
        return $this->arthroshare_format_interval($protocol_step_deadline, $return_type);
    } else {
      return $this->arthroshare_format_interval($protocol_step_deadline, NULL, $format);
    }
  
  }
  
  function arthroshare_order_protocol_steps($steps, $params = array()) {
    
    $steps_ordered = array();
    // dsm($steps);
    $nids = array();
    
    foreach($steps as $key => $step) {
      
      // dsm($step);
      // set the key to the interval and ksort to return ordered node ids;
      if(!isset($params) | empty($params) | $params['node'] == 'Y') {
        $nid_type = ("view nid");
        $nid = $step['raw']['target_id'];
      } else {
        $nid_type = ("cron nid");
        $nid = $step['target_id'];
      }
      
      // add logic for steps due at the same time!!!
      
      $interval = $this->arthroshare_get_protocol_step_deadline($nid, '%d');
      $interval = $interval . "00";
      
      if($interval == $last_interval) {
        $interval = $interval + 1;
      }
      
      // $interval = str_replace(" ", "_", $interval);
      
      $nids[] = array(
          'nid' => $nid, 
          'nid_type' => $nid_type
      );
      
      if(!isset($params) | empty($params)) {
        $steps_ordered[$interval] = $step['raw']['target_id'];
      } elseif($params['node'] == 'Y') {
        $steps_ordered[$interval] = $step;
      } else {
        $steps_ordered[$interval] = array(
            'nid' => $step['target_id'], 
            'deadline' => $this->arthroshare_get_protocol_step_deadline($nid, '%d')
        );
      }
      $last_interval = $interval;
    }
    // dsm($nids);
    ksort($steps_ordered);
    // print_r($steps_ordered);
    return $steps_ordered;
  }
  
  function arthroshare_get_step_info($step_nid, $field_name = NULL, $params = array()) {
    $step_info = node_load($step_nid);
    $return_value = adhd_field_output($field_name, $entity_type = 'node', $step_info);
    
    if(empty($params)) {
      return($return_value);
    } elseif($params['node'] == 'Y') {
      return ($step_info);
    } else {
      return($return_value);
    }
    
    
  }
  
  function arthroshare_get_completed_steps($case_study_nid, $return_type = NULL) {
    
    $completed = array();

    if(node_load($case_study_nid) === FALSE) {
      return $completed;
    }
    
    $node = node_load($case_study_nid);
    
    if($node->field_completed_steps) {
      $completed_steps = $node->field_completed_steps[LANGUAGE_NONE];
    } elseif(!isset($return_type) | $return_type != 'set_step') {
      return $completed;
    }
    
    foreach($completed_steps as $key => $value) {
      $completed[] = $value['target_id'];
    }
    
    if(isset($return_type) && $return_type == 'set_step') {
      return array('node' => $node, 'completed' => $completed);
    }
    return $completed;
  }
  
  function arthroshare_set_completed_step($case_study_nid, $step_nid = NULL) {
   
   if(!isset($case_study_nid)) {
     return("invalid request");
   }
    
   $case_node = $this->arthroshare_get_completed_steps($case_study_nid, 'set_step');
   
   $success = 'NO NODE UPDATED';
   
   if(!isset($case_node['node']->field_completed_steps) | empty($case_node['node']->field_completed_steps)) {
     $case_node['node']->field_completed_steps[LANGUAGE_NONE][]['target_id'] = $step_nid;
     node_save($case_node['node']);
     $success = 'SUCCESS:: Node->' . $case_node['node']->nid . ' set first step';
   } elseif(!in_array($step_nid, $case_node['completed'])) {
     $case_node['node']->field_completed_steps[LANGUAGE_NONE][]['target_id'] = $step_nid;
     node_save($case_node['node']);
     $success = 'SUCCESS:: Node->' . $case_node['node']->nid . ' set next step';
   } elseif (in_array($step_nid, $case_node['completed'])) {
     $success .= ' Duplicate Completed Step';
   }
    
    return($success);
  }
}