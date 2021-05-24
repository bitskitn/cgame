#!/usr/bin/wish

menu .mbar
    . configure -menu .mbar

  # Create the File Menu
    menu .mbar.fl -tearoff 0
    .mbar add cascade -menu .mbar.fl -label File -underline 0
        .mbar.fl add command -label Exit -command { exit }

  # Create the Edit Menu
    menu .mbar.ed -tearoff 0
    .mbar add cascade -menu .mbar.ed -label Edit -underline 0
        .mbar.ed add command -label Undo -underline 0
        .mbar.ed add separator
        .mbar.ed add command -label Cut -underline 1
        .mbar.ed add command -label Copy -underline 0
        .mbar.ed add command -label Paste -underline 0
        .mbar.ed add command -label Delete -underline 0

frame .toolbar -bd 1 -relief raised
  # Camera Naviation Button
    image create photo camicon -file "icons/cam.png"
    button .toolbar.cambtn -image camicon -relief flat -command { exit }
    pack .toolbar.cambtn -side left -padx [list 2 18] -pady 2
  # Transformation Buttons
    image create photo posicon -file "icons/pos.png"
    button .toolbar.posbtn -image posicon -relief flat -command { exit }
    pack .toolbar.posbtn -side left -padx 2 -pady 2
    image create photo roticon -file "icons/rot.png"
    button .toolbar.rotbtn -image roticon -relief flat -command { exit }
    pack .toolbar.rotbtn -side left -padx 2 -pady 2
    image create photo sclicon -file "icons/scl.png"
    button .toolbar.sclbtn -image sclicon -relief flat -command { exit }
    pack .toolbar.sclbtn -side left -padx [list 2 18] -pady 2
  # Game Control Buttons
    image create photo playicon -file "icons/play.png"
    button .toolbar.playbtn -image playicon -relief flat -command { exit }
    pack .toolbar.playbtn -side left -padx 2 -pady 2
    image create photo pauseicon -file "icons/pause.png"
    button .toolbar.pausebtn -image pauseicon -relief flat -command { exit }
    pack .toolbar.pausebtn -side left -padx 2 -pady 2
pack .toolbar -fill x

frame .holder -container 1
pack .holder -fill both -expand yes
exec ../game -e [winfo id .holder] &

frame .statusbar -bd 1 -relief sunken
    label .statusbar.mylabel -text "hello, world!"
    pack .statusbar.mylabel -side left -padx 2 -pady 2
pack .statusbar -fill x

wm title . "CGame Editor"
wm geometry . 800x450+200+200
