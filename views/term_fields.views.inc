<?php

/**
 * Implementation of hook_views_data().
 */
function term_fields_views_data() {
	$fields = term_fields_get_fids();
	if (!empty($fields)) {
		$data = array();
		$data['term_fields_term']['table']['group'] = t('Term Fields');
	
		$data['term_fields_term']['table']['join']['term_data'] = array(
			'left_field' => 'tid',
			'field' => 'tid'
		);
		foreach ($fields as $fid) {
			$sql = "SELECT title, type FROM {term_fields} WHERE fid = '%s'";
			$record = db_fetch_object(db_query($sql, $fid));

			// set handlers. some data types can't be used for certain features of views.
			// datetime fields can't be used for arguments or filters for example.
			// some fields need a special handler to add things like 'Link to this term' functionality
			switch ($record->type) {
				case 'textfield':
					$data['term_fields_term'][$fid] = array(
						'field' => array(
					 	  'handler' => 'term_fields_handler_field'
						),
						'argument' => array(
							'handler' => 'views_handler_argument_string'
						),
						'filter' => array(
							'handler' => 'views_handler_filter_string'
						),
						'sort' => array(
						  'handler' => 'views_handler_sort'
						)
					);
					break;
				case 'textarea':
					$data['term_fields_term'][$fid] = array(
						'field' => array(
					 	  'handler' => 'term_fields_handler_field_textarea'
						),
						'filter' => array(
							'handler' => 'views_handler_filter_string'
						)
					);
					break;
				case 'numeric':
					$data['term_fields_term'][$fid] = array(
						'field' => array(
							'handler' => 'term_fields_handler_field_numeric'
						),
						'argument' => array(
							'handler' => 'views_handler_argument_numeric'
						),
						'filter' => array(
							'handler' => 'views_handler_filter_numeric'
						),
						'sort' => array(
						  'handler' => 'views_handler_sort'
						)
					);
					break;
				case 'select':
					$data['term_fields_term'][$fid] = array(
						'field' => array(
					 	  'handler' => 'term_fields_handler_field'
						),
						'argument' => array(
							'handler' => 'views_handler_argument_string'
						),
						'filter' => array(
							'handler' => 'views_handler_filter_string'
						),
						'sort' => array(
						  'handler' => 'views_handler_sort'
						)
					);
					break;
				case 'date':
					$data['term_fields_term'][$fid] = array(
						'field' => array(
							'handler' => 'term_fields_handler_field_date'
						),
						'filter' => array(
						  'handler' => 'views_handler_filter_date'
						),
						'sort' => array(
						  'handler' => 'views_handler_sort_date'
						)
					);
					break;
				case 'file':
				  $data['term_fields_term'][$fid] = array(
				    'field' => array(
				      'handler' => 'views_handler_field'
				    )
				  );
			}
		
			// all fields get this
			$data['term_fields_term'][$fid]['title'] = $record->title . ' (' . $fid . ')';
			$data['term_fields_term'][$fid]['help'] = t('The value of !fid for this particular term.', array('!fid' => $fid));
		}
	
		return $data;
	}
}

/**
 * Implementation of hook_views_handlers().
 */
function term_fields_views_handlers() {
	return array(
		'info' => array(
			'path' => drupal_get_path('module', 'term_fields') . '/views',
		),
		'handlers' => array(
			'term_fields_handler_field' => array(
				'parent' => 'views_handler_field'
			),
			'term_fields_handler_field_textarea' => array(
				'parent' => 'views_handler_field'
			),
			'term_fields_handler_field_numeric' => array(
				'parent' => 'views_handler_field_numeric'
			),
			'term_fields_handler_field_date' => array(
				'parent' => 'views_handler_field_date'
			)
		)
	);
}