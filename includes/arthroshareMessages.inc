<?php

class arthroshareMessages {
  function arthroshare_send_mail($to, $from, $subject, $message) {
    
    
    $params['subject'] = t($subject);
    $params['body'] = array(
        t($message)
    );
    $from = 'chris@adhdmedia';
    
    $key = 'test-email';
    $mail = drupal_mail('smtp', 'smtp-test', $to, $language, $params);
    dsm($mail);
  }
}