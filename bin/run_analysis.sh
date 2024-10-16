LOG_FILE="../logs/analysis.log"
mkdir -p ../logs

log() {
    echo "$(date) : $1" | tee -a $LOG_FILE
}

log "Starting large-scale STA analysis"

log "Running Perl script to check STA violations in large report..."
perl src/check_sta_violations.pl
if [ $? -ne 0 ]; then
    log "Error: Perl script failed to check STA violations."
    exit 1
fi

log "Launching Python PyQt GUI..."
python3 src/gui_result.py
if [ $? -ne 0 ]; then
    log "Error: Failed to launch Python GUI."
    exit 1
fi

log "STA analysis completed successfully."
