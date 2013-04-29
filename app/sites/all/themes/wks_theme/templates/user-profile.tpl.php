<div class="profile-box">
  
  <div class="profile-picture">
    <?php print (render($user_profile['user_picture'])); ?>
  </div>
  <div class="profile-data">
    <ul>
      <li>
        <?php print (render($user_profile['field_adressfield'])); ?>
      </li>
      <li>
        <?php print (render($user_profile['field_telefon'])); ?>
      </li>
      <li>
        <?php print (render($user_profile['field_dzielnica'])); ?>
      </li>
      <li>
        <?php print (render($user_profile['field_samoch_d'])); ?>
      </li>
      <li>
        <?php print (render($user_profile['field_prawo_jazdy'])); ?>
      </li>
      <li>
        <?php print (render($user_profile['summary']['invited_by'])); ?>
      </li>
         
    </ul>
  </div>
  
</div>


