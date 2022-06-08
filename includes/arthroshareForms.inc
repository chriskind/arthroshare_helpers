<?php
/*
 * TO DO:
 * Class some of the arthroshare functions for webforms
 * 
 */
class arthroshareForms {
  
  public $survey_gizmo_user;
  public $survey_gizmo_pass;
  public $survey_gizmo_link;
  public $survey_gizmo_uid;
  
  public $case_study_nid;
  public $protocol_step;
  public $protocol_nid;
  public $survey_id;
  public $step_nid;
  
  function __construct() {
    $this->survey_gizmo_user = variable_get('survey_gizmo_user', NULL);
    $this->survey_gizmo_pass = variable_get('survey_gizmo_pass', NULL);
    $this->survey_gizmo_link = variable_get('survey_gizmo_link', NULL);
    $this->survey_gizmo_uid = variable_get('survey_gizmo_uid', NULL);
  }
  
  function build_survey_gizmo_form_embed() {
    
    // Check to make sure it's not too early to do the form
    
    $return_form = "<iframe src='http://www.surveygizmo.com/s3/" . $this->survey_id . "?case_study_nid=" . $this->case_study_nid . "&step_nid=" . $this->step_nid . "&protocol_nid=" . $this->protocol_nid . "' frameborder='0' width='100%' height='500' style='overflow:hidden'></iframe>";
    return $return_form;
  }
  
  
  function arthroshare_select_options() {
    
    $likert_1 = array(
        'Never', 
        'Rarely', 
        'Sometimes', 
        'Often', 
        'Always'
    );
    
    $limit_3 = array(
        'Yes, Limited A Lot', 
        'Yes, Limited A Little', 
        'No, Not Limited At All'
    );
    
    $yes_no = array(
        'Yes', 
        'No'
    );
    
    $allofthetime_noneofthetime = array(
        'All of the time', 
        'Most of the time', 
        'A good bit of the time', 
        'A little of the time', 
        'None of the time'
    );
    
    $true_to_false = array(
        'Definitely True', 
        'Mostly True', 
        'Don\'t know', 
        'Mostly False', 
        'Definitely False'
    );
    $one_to_ten = $this->numeric_scale(1, 10);
    
    $notatall_to_extrememly = array(
        'Not at all', 
        'Mildly', 
        'Moderately', 
        'Very', 
        'Extremely'
    );
    
    $difficulty = array(
        'Not difficult at all', 
        'Minimally difficult', 
        'Moderately difficult', 
        'Extremely difficult', 
        'Unable to do'
    );
    
    $scales = array(
        'likert_1' => array(
            'title' => 'Likert 1', 
            'scale_options' => $likert_1
        ), 
        'limit_3' => array(
            'title' => 'Limit 3', 
            'scale_options' => $limit_3
        ), 
        'yes_no' => array(
            'title' => 'Yes/No', 
            'scale_options' => $yes_no
        ), 
        'allofthetime_noneofthetime' => array(
            'title' => '(All to None) of the time', 
            'scale_options' => $allofthetime_noneofthetime
        ), 
        'true_to_false' => array(
            'title' => 'True to False', 
            'scale_options' => $true_to_false
        ), 
        'one_to_ten' => array(
            'title' => '1 to 10', 
            'scale_options' => $one_to_ten
        ), 
        'difficulty_1' => array(
            'title' => 'Difficutly 1', 
            'scale_options' => $difficulty
        )
    );
    
    // is this line needed?? 
    $items = array();
    
    foreach($scales as $key => $scale) {
      
      $options = $scale['scale_options'];
      // adhd_buffer_to_dsm($options);
      

      $items[$key] = array(
          'title' => t($scale['title']), 
          'options callback' => array($this, 'build_scale_1'), 
          'options arguments' => $options
      );
    }
    
    $items['mosby_scale_one'] = array(
        'title' => t('Mosby Scale One'), 
        'options callback' => array($this, 'mosby_scale_one'),
    );
    
    // adhd_buffer_to_dsm($items);
    return $items;
  
  }
  
  function mosby_scale_one() {
    
    $scale = array(
        '1' => 'Never', 
        '2' => 'Rarely', 
        '3' => 'Sometimes', 
        '4' => 'Often', 
        '5' => 'Always'
    );
    
    return $scale;
  }
  
  function build_scale_1($component, $flat, $arguments) {
    
    $message = 'function';
    
    // adhd_buffer_to_dsm($arguments);
    $options = $arguments;
    // $options = array('no', 'values sent');
    

    $scale_values = array();
    
    foreach($options as $option) {
      $scale_values[] = $option;
    }
    // adhd_buffer_to_dsm($scale_values);
    return $scale_values;
  }
  
  function numeric_scale($start, $stop, $reverse = FALSE) {
    
    $i = $start;
    $numbers = array();
    
    while($i <= $stop ) {
      $numbers[] = $i;
      $i++;
    }
    return $numbers;
  }
}
