<?php

class arthroshareCron {
  
  public $date_interval;
  
  function check_all_open_cases($message_node = 37) {
    
    $arthroshareDatesStepsAlerts = new arthroshareDatesStepsAlerts();
    
    $query = new EntityFieldQuery();
    $query->entityCondition('entity_type', 'node')->entityCondition('bundle', 'case_study');
    
    // ->propertyCondition('status', 'open');
    

    $result = $query->execute();
    
    $case_status = array();
    $notification_keys = array();
    
    if(!empty($result['node'])) {
      $case_studies = array_keys($result['node']);
      
      foreach($case_studies as $case_study) {
        
        // load case study info
        $case_study_info = node_load($case_study, NULL, TRUE);
        $case_start_date = arthroshare_get_case_field_value($case_study_info->nid, 'field_start_date');
        $alert_levels = $arthroshareDatesStepsAlerts->arthroshare_get_alert_levels($case_study_info->nid);
        $alert_level = -1;
        
        // get user id info + profile
        $patient_profile_id = arthroshare_get_case_field_value($case_study, 'field_subject_id');
        $user_info = user_load($patient_profile_id->uid);
        $user_profile = profile2_load_by_user($patient_profile_id->uid, 'patient');
        
        $protocol_nid = $case_study_info->field_protocol[LANGUAGE_NONE][0]['target_id'];
        $protocol_info = node_load($protocol_nid, NULL, TRUE);
        
        $completed_steps = $arthroshareDatesStepsAlerts->arthroshare_get_completed_steps($case_study_info->nid);
        
        $protocol_steps = $arthroshareDatesStepsAlerts->arthroshare_get_protocol_steps($protocol_nid);
        
        /* May be used to customize message about next step
        foreach ($protocol_steps as $protocol_step) {
          
        }
        */
        
        $protocol_steps_ordered = $arthroshareDatesStepsAlerts->arthroshare_order_protocol_steps($protocol_steps, $params = array(
            'deadline_info'
        ));
        
        $next_step_due_key = NULL;
        
        foreach($protocol_steps_ordered as $key => $step) {
          
          if(in_array($step['nid'], $completed_steps)) {
            $protocol_steps_ordered[$key]['completed'] = 'Y';
          } else {
            $deadline_interval = $arthroshareDatesStepsAlerts->arthroshare_get_protocol_step_deadline($step['nid'], NULL, 'raw');
            $protocol_steps_ordered[$key]['completed'] = 'N';
            $protocol_steps_ordered[$key]['due_date_info'] = $arthroshareDatesStepsAlerts->arthroshare_get_due_date($case_start_date, $deadline_interval);
            
            if(empty($next_step_due_key)) {
              $next_step_due_key = $key;
              if($protocol_steps_ordered[$key]['due_date_info']['in_days'] < 5) {
                $in_days = $protocol_steps_ordered[$key]['due_date_info']['in_days'];
                $alert_level = $arthroshareDatesStepsAlerts->arthroshare_get_alert_level((int)$in_days, $alert_levels);
                $alert_message = node_load($message_node);
                $message_out = array(
                    'alert_message' => $alert_message, 
                    'alert_level' => $alert_level,
                    'alert_info' => $alert_levels[$alert_level]
                );
                $notifications[] = $case_study;
              } else {
                $message_out = array();
              }
            }
          }
        }
        
        // dsm($protocol_steps_ordered);
        

        $case_status[$case_study] = array(
            'case_study_info' => $case_study_info,
            'protocol_info' => $protocol_info,
            'user_info' => $user_info,
            'user_profile' => $user_profile,
            'case_study_nid' => $case_study, 
            'case_start_date' => $case_start_date, 
            'patient_profile_id' => $patient_profile_id->uid, 
            'user_mail' => $user_info->mail, 
            'protocol_steps' => $protocol_steps, 
            'protocol_steps_ordered' => $protocol_steps_ordered, 
            'completed_steps' => $completed_steps, 
            'next_step_due_info' => $next_step_due_key,
            'message_out' => $message_out
        );
        
      // watchdog('arthroshare cron', 'send message', 'error message');
      }
    }
    // dsm($case_status);
    

    return (array(
        'case_status' => $case_status, 
        'notifications' => $notifications
    ));
  }
}